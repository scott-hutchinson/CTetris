#include "tetris.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "terminal.h"
#include "input.h"


static void init_screen(void)
{
    Terminal_begin_raw_mode();
    Terminal_reset_cursor(0);
    Terminal_clear_screen(1);
}

static void cleanup_screen(void)
{
    Terminal_reset_cursor(1);
    Terminal_end_raw_mode();
    Terminal_clear_screen(1);
}

static int collision(int collision_type, Block *block, Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (Block_get_coord_x(block, collision_type, i) == COORDINATE_IGNORE
            || Block_get_coord_y(block, collision_type, i) == COORDINATE_IGNORE
            || block->y + Block_get_coord_y(block, collision_type, i) < 0
        ) {
            continue;
        }

        if (Buffer_get_pixel_enabled(
                buffer,
                block->x + (unsigned int) Block_get_coord_x(block,
                                                            collision_type, i),
                block->y + (unsigned int) Block_get_coord_y(block,
                                                            collision_type, i))
        ) {
            return 1;
        }
    }

    return 0;
}

static int draw_collision(Block *block, Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (Buffer_get_pixel_enabled(
                buffer,
                block->x + (unsigned int) Block_get_coord_x(block,
                                                            COORDINATE_MAIN, i),
                block->y + (unsigned int) Block_get_coord_y(block,
                                                            COORDINATE_MAIN, i))
        ) {
            return 1;
        }
    }

    return 0;
}

static int rotate_collision(Block *block, Buffer *buffer)
{
    if (block->type == BLOCK_O) {
        return 0;
    }

    uint8_t original_rotate = block->rotate;

    Block_rotate(block);

    if (collision(COORDINATE_BOTTOM_COLLISION, block, buffer)
        || draw_collision(block, buffer)
    ) {
        Block_set_rotate(block, original_rotate);

        return 1;
    }

    Block_set_rotate(block, original_rotate);

    return 0;
}

static void drop_line(Buffer *buffer, int line_number)
{
    unsigned int i;
    for (i = 1; i < buffer->width-1; i++) {
        Buffer_set_pixel(buffer,
                         i, (unsigned int) line_number+1,
                         buffer->pixel_data[line_number][i]);
    }
}

static void erase_line(Renderer *renderer, int line_number)
{
    int i;
    for (i = 1; i < (int) renderer->buffer->width-1; i++) {
        Buffer_set_pixel_enabled(renderer->buffer,
                                 (unsigned int) i, (unsigned int) line_number,
                                 0);
    }

    for (i = 1; i < (int) renderer->row_floor-1; i++) {
        if (line_number - i > 0) {
            drop_line(renderer->buffer, line_number - i);
        }
    }
}

static int check_complete_lines(Tetris *tetris)
{
    int empty_cell_count = 0, line_count = 0;

    unsigned int check_count, x, y;
    for (check_count = 0; check_count < 4; check_count++) {
        for (y = tetris->renderer->row_floor-1; y > 0; y--) {
            for (x = 1; x < tetris->renderer->buffer->width-1; x++) {
                if (!Buffer_get_pixel_enabled(tetris->renderer->buffer,
                                              x, y)
                ) {
                    empty_cell_count++;
                }
            }

            if (empty_cell_count == 0) {
                line_count++;
                erase_line(tetris->renderer, (int) y);
                tetris->lines_completed++;
                Renderer_draw_panel_line_counter(tetris->renderer,
                                                 tetris->lines_completed);
            }
            empty_cell_count = 0;
        }
    }

    tetris->renderer->buffer->dirty = 1;

    return line_count;
}

static unsigned int random_in_range(unsigned int min, unsigned int max)
{
    int base_random = rand();

    if (RAND_MAX == base_random) {
        return random_in_range(min, max);
    }

    unsigned int range     = max - min;
    unsigned int remainder = RAND_MAX % range;
    unsigned int bucket    = RAND_MAX / range;

    if ((unsigned int) base_random < RAND_MAX - remainder) {
        return min + ((unsigned int) base_random / bucket);
    }

    return random_in_range(min, max);
}


static unsigned int get_random_block_type(void)
{
    unsigned int min = 1, max = 8;

    return random_in_range(min, max);
}

static void set_ghost_block(Tetris *tetris)
{
    tetris->ghost_block->x = tetris->current_block->x;
    tetris->ghost_block->y = tetris->current_block->y;

    Block_set_type(tetris->ghost_block,
                   tetris->current_block->type,
                   tetris->current_block->rotate);

    tetris->ghost_block->color = COLOR_GRAY;

    while (!collision(COORDINATE_BOTTOM_COLLISION,
                             tetris->ghost_block,
                             tetris->renderer->buffer)
    ) {
        tetris->ghost_block->y++;
    }
}

static void next_block(Tetris *tetris)
{
    tetris->current_block->x = 5;
    tetris->current_block->y = 0;

    Block_set_type(tetris->current_block, get_random_block_type(), 0);

    if (tetris->enable_ghost_block == 1) {
        set_ghost_block(tetris);
    }
}

static void check_game_over(Tetris *tetris)
{
    if (draw_collision(tetris->current_block, tetris->renderer->buffer)) {
        Renderer_draw_game_over(tetris->renderer);

        Tetris_destroy(tetris);

        exit(0);
    }
}

static void update_score(Tetris *tetris, int line_count)
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

static void update_level(Tetris *tetris)
{
    if (tetris->lines_until_level_up <= 0) {
        tetris->lines_until_level_up += 10;

        tetris->level++;

        if (tetris->gravity_frame_delay > 1) {
            tetris->gravity_frame_delay--;
        }
    }
}

static void get_key_input(Tetris *tetris)
{
    tetris->key_left        = 0;
    tetris->key_right       = 0;
    tetris->key_down        = 0;
    tetris->key_drop        = 0;
    tetris->key_rotate      = 0;
    tetris->key_quit        = 0;
    tetris->key_pause       = 0;
    tetris->key_ghost_block = 0;

    tetris->current_key = Input_get_key(tetris->current_key_sequence);

    if (tetris->current_key == KEY_Q
        || tetris->current_key == KEY_CONTROL_C
    ) {
        tetris->key_quit = 1;
    }
    else if (tetris->current_key == KEY_P) {
        tetris->key_pause = 1;
    }
    else if (tetris->current_key == KEY_G) {
        tetris->key_ghost_block = 1;
    }
    else if (tetris->current_key == KEY_SPACE) {
        tetris->key_drop = 1;
    }
    else if (tetris->current_key == KEY_UP) {
        tetris->key_rotate = 1;
    }
    else if (tetris->current_key == KEY_LEFT) {
        tetris->key_left = 1;
    }
    else if (tetris->current_key == KEY_RIGHT) {
        tetris->key_right = 1;
    }
    else if (tetris->current_key == KEY_DOWN) {
        tetris->key_down = 1;
    }
}

static void update(Tetris *tetris)
{
    if (tetris->key_quit) {
        Tetris_destroy(tetris);
        exit(0);
    }
    else if (tetris->key_pause) {
        if (tetris->game_state == RUNNING) {
            tetris->game_state = PAUSED;
            Renderer_erase_panel_labels(tetris->renderer);
            Renderer_erase_panel_line_counter(tetris->renderer);
            Renderer_erase_panel_score(tetris->renderer);
            Renderer_erase_panel_level(tetris->renderer);

            Renderer_draw_pause_message(tetris->renderer);
        }
        else if (tetris->game_state == PAUSED) {
            tetris->game_state = RUNNING;
            Renderer_erase_pause_message(tetris->renderer);

            Renderer_draw_panel_labels(tetris->renderer);
            Renderer_draw_panel_line_counter(tetris->renderer,
                                             tetris->lines_completed);
            Renderer_draw_panel_score(tetris->renderer, tetris->score);
            Renderer_draw_panel_level(tetris->renderer, tetris->level);
        }
    }

    if (tetris->game_state != PAUSED) {
        if (collision(COORDINATE_BOTTOM_COLLISION,
                      tetris->current_block,
                      tetris->renderer->buffer)
        ) {
            tetris->movement_frame_counter++;
            if (tetris->movement_frame_counter == tetris->movement_frame_delay) {
                tetris->movement_frame_counter = 0;

                Renderer_draw_block(tetris->renderer, tetris->current_block);

                int complete_lines = check_complete_lines(tetris);

                next_block(tetris);

                check_game_over(tetris);

                update_score(tetris, complete_lines);
                update_level(tetris);

                Renderer_draw_panel_score(tetris->renderer, tetris->score);
                Renderer_draw_panel_level(tetris->renderer, tetris->level);

                tetris->renderer->buffer->dirty = 1;
            }
        }

        if (!collision(COORDINATE_BOTTOM_COLLISION,
                       tetris->current_block,
                       tetris->renderer->buffer)
        ) {
            tetris->gravity_frame_counter++;
            if (tetris->gravity_frame_counter == tetris->gravity_frame_delay) {
                tetris->gravity_frame_counter = 0;
                tetris->current_block->y++;
                tetris->renderer->buffer->dirty = 1;
            }
        }
        if (tetris->key_drop) {
            while (!collision(COORDINATE_BOTTOM_COLLISION,
                              tetris->current_block,
                              tetris->renderer->buffer)
            ) {
                tetris->current_block->y++;
                tetris->score += 2;
            }
            tetris->renderer->buffer->dirty = 1;
        }
        else if (tetris->key_rotate
                 && rotate_collision(tetris->current_block,
                                     tetris->renderer->buffer) != 1
        ) {
            Block_rotate(tetris->current_block);
            if (tetris->enable_ghost_block == 1) {
                set_ghost_block(tetris);
            }
            tetris->renderer->buffer->dirty = 1;
        }
        else if (tetris->key_ghost_block) {
            if (tetris->enable_ghost_block) {
                tetris->enable_ghost_block = 0;
            }
            else {
                tetris->enable_ghost_block = 1;
                set_ghost_block(tetris);
            }
            tetris->renderer->buffer->dirty = 1;
        }
        else if (tetris->key_left
                 && !collision(COORDINATE_LEFT_COLLISION,
                               tetris->current_block,
                               tetris->renderer->buffer)
        ) {
            tetris->current_block->x--;
            if (tetris->enable_ghost_block == 1) {
                set_ghost_block(tetris);
            }
            tetris->renderer->buffer->dirty = 1;
        }
        else if (tetris->key_right
                 && !collision(COORDINATE_RIGHT_COLLISION,
                               tetris->current_block,
                               tetris->renderer->buffer)
        ) {
            tetris->current_block->x++;

            if (tetris->enable_ghost_block == 1) {
                set_ghost_block(tetris);
            }

            tetris->renderer->buffer->dirty = 1;
        }
        else if (tetris->key_down
                 && !collision(COORDINATE_BOTTOM_COLLISION,
                               tetris->current_block,
                               tetris->renderer->buffer)
        ) {
            tetris->current_block->y++;

            if (tetris->enable_ghost_block == 1) {
                set_ghost_block(tetris);
            }

            tetris->score++;

            Renderer_draw_panel_score(tetris->renderer, tetris->score);

            tetris->renderer->buffer->dirty = 1;
        }
    }
}

static void draw_frame(Tetris *tetris)
{
    if (tetris->enable_ghost_block) {
        Renderer_draw_block(tetris->renderer, tetris->ghost_block);
    }
    Renderer_draw_block(tetris->renderer, tetris->current_block);

    Renderer_present_buffer(tetris->renderer);

    if (tetris->enable_ghost_block) {
        Renderer_erase_block(tetris->renderer, tetris->ghost_block);
    }
    Renderer_erase_block(tetris->renderer, tetris->current_block);
}

void Tetris_sleep_ms(unsigned int milliseconds)
{
    time_t seconds = (int) (milliseconds / 1000);

    milliseconds = milliseconds - ((unsigned int) seconds * 1000);

    struct timespec req;
    req.tv_sec = seconds;
    req.tv_nsec = (int) milliseconds * 1000000L;

    while (nanosleep(&req, &req) == -1);
}

Tetris *Tetris_create(void)
{
    srand((unsigned int) time(NULL));

    init_screen();

    Tetris *tetris = malloc(sizeof(Tetris));

    tetris->renderer = Renderer_create(12, 24);

    tetris->current_block = Block_create();
    tetris->ghost_block = Block_create();

    tetris->current_block->x = 5;
    tetris->current_block->y = 0;

    Block_set_type(tetris->current_block, get_random_block_type(), 0);

    tetris->enable_ghost_block = 1;

    tetris->movement_frame_counter = 0;
    tetris->movement_frame_delay   = 10;

    tetris->lines_completed      = 0;
    tetris->lines_until_level_up = 10;

    tetris->score = 0;
    tetris->level = 1;

    tetris->gravity_frame_counter = 0;
    if (tetris->level < 16) {
        tetris->gravity_frame_delay = (uint8_t) (16 - tetris->level);
    }
    else {
        tetris->gravity_frame_delay = 1;
    }

    tetris->key_left        = 0;
    tetris->key_right       = 0;
    tetris->key_down        = 0;
    tetris->key_drop        = 0;
    tetris->key_rotate      = 0;
    tetris->key_quit        = 0;
    tetris->key_pause       = 0;
    tetris->key_ghost_block = 0;

    tetris->game_state = RUNNING;

    Renderer_draw_game_border(tetris->renderer);
    Renderer_draw_panel_labels(tetris->renderer);
    Renderer_draw_panel_line_counter(tetris->renderer, tetris->lines_completed);
    Renderer_draw_panel_score(tetris->renderer, tetris->score);
    Renderer_draw_panel_level(tetris->renderer, tetris->level);

    if (tetris->enable_ghost_block == 1) {
        set_ghost_block(tetris);
    }

    tetris->renderer->buffer->dirty = 1;

    return tetris;
}

void Tetris_destroy(Tetris *tetris)
{
    Renderer_destroy(tetris->renderer);
    Block_destroy(tetris->current_block);
    Block_destroy(tetris->ghost_block);
    free(tetris);

    cleanup_screen();
}

void Tetris_init_options(Tetris *tetris, int option_count, char **options)
{
    tetris->option_count = option_count;
    tetris->options = options;
}

void Tetris_game_loop(Tetris *tetris)
{
    while (1) {
        get_key_input(tetris);

        update(tetris);

        if (tetris->renderer->buffer->dirty) {
            draw_frame(tetris);
        }

        Tetris_sleep_ms(30);
    }
}
