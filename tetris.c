#include "tetris.h"

#include <stdlib.h>
#include <time.h>


Tetris *Tetris_create(void)
{
    srand(time(NULL));
    Tetris_init_screen();
    Tetris *tetris = malloc(sizeof(Tetris));

    // tetris->renderer = Renderer_create(12, 24, XTERM_256);


    tetris->buffer = Buffer_create(12, 24);
    Buffer_fill(tetris->buffer, 0);
    tetris->current_block = Block_create();
    tetris->ghost_block = Block_create();
    tetris->current_block->x = 5;
    tetris->current_block->y = 0;
    Block_set_type(tetris->current_block, Tetris_get_random_block_type(), 0);

    tetris->enable_ghost_block     = 1;
    tetris->gravity_frame_counter  = 0;
    tetris->movement_frame_counter = 0;
    tetris->movement_frame_delay   = 10;
    tetris->lines_completed       = 0;
    tetris->lines_until_level_up    = 10;
    tetris->score                = 0;
    tetris->level                = 1;
    if (tetris->level < 16) {
        tetris->gravity_frame_delay = 16 - tetris->level;
    }
    else {
        tetris->gravity_frame_delay = 1;
    }

    tetris->game_state = RUNNING;
    tetris->color_mode = XTERM_256;
    tetris->buffer->data[ROW_LINE_COUNTER][0] = 1;
    tetris->buffer->data[ROW_SCORE][0] = 1;
    tetris->buffer->data[ROW_LEVEL][0] = 1;
    Renderer_draw_game_border(tetris->buffer);

    if (tetris->enable_ghost_block == 1) {
        Tetris_set_ghost_block(tetris);
    }

    tetris->buffer->dirty = 1;

    return tetris;
}

void Tetris_destroy(Tetris *tetris)
{
    // Renderer_destroy(tetris->renderer);
    Buffer_destroy(tetris->buffer);
    Block_destroy(tetris->current_block);
    Block_destroy(tetris->ghost_block);
    free(tetris);
    Tetris_cleanup_screen();
}

void Tetris_init_screen(void)
{
    Terminal_begin_raw_mode();
    Terminal_set_cursor(0);
    Terminal_clear_screen(1);
}

void Tetris_cleanup_screen(void)
{
    Terminal_set_cursor(1);
    Terminal_end_raw_mode();
    Terminal_clear_screen(1);
}

void Tetris_game_loop(Tetris *tetris)
{
    while (1) {
        Tetris_get_key_input(tetris);
        Tetris_update(tetris);
        if (tetris->buffer->dirty) {
            Tetris_draw_frame(tetris);
        }
        Tetris_sleep_ms(50);
    }
}

void Tetris_get_key_input(Tetris *tetris)
{
    tetris->current_key = Input_get_key(tetris->current_key_sequence);
}

void Tetris_update(Tetris *tetris)
{
    if (Tetris_collision(BOTTOM_COLLISION, tetris->current_block, tetris->buffer)) {
        tetris->movement_frame_counter++;
        if (tetris->movement_frame_counter == tetris->movement_frame_delay) {
            tetris->movement_frame_counter = 0;
            Renderer_draw_block(tetris->buffer, tetris->current_block);
            Tetris_next_block(tetris);
            int complete_lines = Tetris_check_complete_lines(tetris);
            Tetris_check_game_over(tetris);
            Tetris_update_score(tetris, complete_lines);
            Tetris_update_level(tetris);
            tetris->buffer->dirty = 1;
        }
    }

    if (tetris->current_key == KEY_Q
        || tetris->current_key == KEY_CONTROL_C
    ) {
        Tetris_destroy(tetris);
        exit(0);
    }
    else if (tetris->current_key == KEY_P) {
        if (tetris->game_state == RUNNING) {
            tetris->game_state = PAUSED;
            Renderer_draw_pause_message(tetris->buffer);
        }
        else if (tetris->game_state == PAUSED) {
            tetris->game_state = RUNNING;
            Renderer_erase_pause_message(tetris->buffer);
        }
    }
    else if (tetris->current_key == KEY_G) {
        if (tetris->enable_ghost_block) {
            tetris->enable_ghost_block = 0;
        }
        else {
            tetris->enable_ghost_block = 1;
            if (tetris->enable_ghost_block == 1) {
                Tetris_set_ghost_block(tetris);
            }
        }
        tetris->buffer->dirty = 1;
    }


    if (tetris->game_state != PAUSED) {
        if (!Tetris_collision(BOTTOM_COLLISION, tetris->current_block, tetris->buffer)) {
            tetris->gravity_frame_counter++;
            if (tetris->gravity_frame_counter == tetris->gravity_frame_delay) {
                tetris->gravity_frame_counter = 0;
                tetris->current_block->y++;
                tetris->buffer->dirty = 1;
            }
        }
        if (tetris->current_key == KEY_SPACE) {
            while (!Tetris_collision(BOTTOM_COLLISION, tetris->current_block, tetris->buffer)) {
                tetris->current_block->y++;
                tetris->score += 2;
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->current_key == KEY_UP && Tetris_rotate_collision(tetris->current_block, tetris->buffer) != 1) {
            Block_rotate(tetris->current_block);
            if (tetris->enable_ghost_block == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->current_key == KEY_LEFT
                 && !Tetris_collision(LEFT_COLLISION, tetris->current_block, tetris->buffer)) {
            tetris->current_block->x--;
            if (tetris->enable_ghost_block == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->current_key == KEY_RIGHT
                 && !Tetris_collision(RIGHT_COLLISION, tetris->current_block, tetris->buffer)
        ) {
            tetris->current_block->x++;
            if (tetris->enable_ghost_block == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->current_key == KEY_DOWN
                 && !Tetris_collision(BOTTOM_COLLISION, tetris->current_block, tetris->buffer)
        ) {
            tetris->current_block->y++;
            if (tetris->enable_ghost_block == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->score++;

//          if (tetris->enable_ghost_block == 1) {
//              Tetris_set_ghost_block(tetris);
//          }
            tetris->buffer->dirty = 1;
        }
    }
}

void Tetris_draw_frame(Tetris *tetris)
{
    Renderer_draw_game(
        tetris->buffer,
        tetris->current_block,
        tetris->ghost_block,
        tetris->color_mode,
        tetris->lines_completed,
        tetris->score,
        tetris->level,
        tetris->enable_ghost_block
    );
}

int Tetris_check_complete_lines(Tetris *tetris)
{
    int empty_cell_count = 0, line_count = 0;
    int check_count, x, y;
    for (check_count = 0; check_count < 4; check_count++) {
        for (y = ROW_FLOOR-1; y > 0; y--) {
            for (x = 1; x < BUFFER_WIDTH-1; x++) {
                if (tetris->buffer->data[y][x] == EMPTY) {
                    empty_cell_count++;
                }
            }
            if (empty_cell_count == 0) {
                line_count++;
                Tetris_erase_line(tetris->buffer, y);
                tetris->lines_completed++;
            }
            empty_cell_count = 0;
        }
    }
    tetris->buffer->dirty = 1;
    return line_count;
}

void Tetris_erase_line(Buffer *buffer, int line_number)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        Buffer_set_cell(buffer, i, line_number, EMPTY);
    }
    for (i = 1; i < ROW_FLOOR-1; i++) {
        if (line_number - i > 0) {
            Tetris_drop_line(buffer, line_number - i);
        }
    }
}

void Tetris_drop_line(Buffer *buffer, int line_number)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        Buffer_set_cell(buffer, i, line_number+1, buffer->data[line_number][i]);
        Buffer_set_cell(buffer, i, line_number, EMPTY);
    }
}

int Tetris_collision(int collision_type, Block *block, Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (Block_get_coord_x(block, collision_type, i) == IGNORE
            || Block_get_coord_y(block, collision_type, i) == IGNORE
            || block->y + Block_get_coord_y(block, collision_type, i) < 0
        ) {
            continue;
        }
        if (Buffer_get_cell(
                buffer,
                block->x + Block_get_coord_x(block, collision_type, i),
                block->y + Block_get_coord_y(block, collision_type, i)
            ) != EMPTY
        ) {
            return 1;
        }
    }
    return 0;
}

int Tetris_rotate_collision(Block *block, Buffer *buffer)
{
    if (block->type == BLOCK_O) {
        return 0;
    }
    int original_rotate = block->rotate;
    Block_rotate(block);
    if (Tetris_collision(BOTTOM_COLLISION, block, buffer)
        || Tetris_draw_collision(block, buffer)
    ) {
        Block_set_rotate(block, original_rotate);
        return 1;
    }
    Block_set_rotate(block, original_rotate);
    return 0;
}

int Tetris_draw_collision(Block *block, Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (Buffer_get_cell(
                buffer,
                block->x + Block_get_coord_x(block, MAIN, i),
                block->y + Block_get_coord_y(block, MAIN, i)
            ) != EMPTY
            && Buffer_get_cell(
                buffer,
                block->x + Block_get_coord_x(block, MAIN, i),
                block->y + Block_get_coord_y(block, MAIN, i)
            ) != -1
        ) {
            return 1;
        }
    }
    return 0;
}

void Tetris_next_block(Tetris *tetris)
{
    tetris->current_block->x = 5;
    tetris->current_block->y = 0;
    Block_set_type(tetris->current_block, Tetris_get_random_block_type(), 0);

    if (tetris->enable_ghost_block == 1) {
        Tetris_set_ghost_block(tetris);
    }
}

int Tetris_get_random_block_type(void)
{
    int min = 1, max = 7;
    return min + (rand() % (int)(max - min + 1));
}

void Tetris_set_ghost_block(Tetris *tetris)
{
    tetris->ghost_block->x = tetris->current_block->x;
    tetris->ghost_block->y = tetris->current_block->y;
    Block_set_type(tetris->ghost_block, tetris->current_block->type, tetris->current_block->rotate);
    tetris->ghost_block->fill_type = FILL_GHOST;
    while (!Tetris_collision(BOTTOM_COLLISION, tetris->ghost_block, tetris->buffer)) {
        tetris->ghost_block->y++;
    }
}

void Tetris_update_score(Tetris *tetris, int line_count)
{
    if (line_count == 1) {
        tetris->score += 100 * tetris->level;
        tetris->lines_until_level_up -= 1;
    }
    else if (line_count == 2) {
        tetris->score += 300 * tetris->level;
        tetris->lines_until_level_up -= 3;
    }
    else if (line_count == 3) {
        tetris->score += 500 * tetris->level;
        tetris->lines_until_level_up -= 5;
    }
    else if (line_count == 4) {
        tetris->score += 800 * tetris->level;
        tetris->lines_until_level_up -= 8;
    }
}

void Tetris_update_level(Tetris *tetris)
{
    if (tetris->lines_until_level_up <= 0) {
        tetris->lines_until_level_up += 10;
        tetris->level++;
        if (tetris->gravity_frame_delay > 1) {
            tetris->gravity_frame_delay--;
        }
    }
}

void Tetris_check_game_over(Tetris *tetris)
{
    if (Tetris_draw_collision(tetris->current_block, tetris->buffer)) {
        Renderer_draw_game_over(
            tetris->buffer,
            tetris->color_mode,
            tetris->lines_completed,
            tetris->score,
            tetris->level
        );

        Tetris_destroy(tetris);
        exit(0);
    }
}

void Tetris_sleep_ms(unsigned int milliseconds)
{
    struct timespec req = {0};

    req.tv_sec = 0;
    req.tv_nsec = milliseconds * 1000000L;

    while (nanosleep(&req, &req) == -1);
}
