#include "tetris.h"

#include <stdlib.h>
#include <time.h>


Tetris *Tetris_create(void)
{
    srand(time(NULL));
    Tetris_init_screen();
    Tetris *tetris = malloc(sizeof(Tetris));

    tetris->renderer = Renderer_create(12, 24, XTERM_256);


    tetris->buffer = Buffer_create(12, 24);
    Buffer_fill(tetris->buffer, 0);
    tetris->currentBlock = Block_create();
    tetris->ghostBlock = Block_create();
    tetris->currentBlock->x = 5;
    tetris->currentBlock->y = 0;
    Block_set_type(tetris->currentBlock, Tetris_get_random_block_type(), 0);

    tetris->enableGhostBlock     = 1;
    tetris->gravityFrameCounter  = 0;
    tetris->movementFrameCounter = 0;
    tetris->movementFrameDelay   = 10;
    tetris->linesCompleted       = 0;
    tetris->linesUntilLevelUp    = 10;
    tetris->score                = 0;
    tetris->level                = 1;
    if (tetris->level < 16) {
        tetris->gravityFrameDelay = 16 - tetris->level;
    }
    else {
        tetris->gravityFrameDelay = 1;
    }

    tetris->gameState = RUNNING;
    tetris->colorMode = XTERM_256;
    tetris->buffer->data[ROW_LINE_COUNTER][0] = 1;
    tetris->buffer->data[ROW_SCORE][0] = 1;
    tetris->buffer->data[ROW_LEVEL][0] = 1;
    Renderer_draw_game_border(tetris->buffer);

    if (tetris->enableGhostBlock == 1) {
        Tetris_set_ghost_block(tetris);
    }

    tetris->buffer->dirty = 1;

    return tetris;
}

void Tetris_destroy(Tetris *tetris)
{
    Renderer_destroy(tetris->renderer);
    Buffer_destroy(tetris->buffer);
    Block_destroy(tetris->currentBlock);
    Block_destroy(tetris->ghostBlock);
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
    tetris->currentKey = Input_get_key(tetris->currentKeySequence);
}

void Tetris_update(Tetris *tetris)
{
    if (Tetris_collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
        tetris->movementFrameCounter++;
        if (tetris->movementFrameCounter == tetris->movementFrameDelay) {
            tetris->movementFrameCounter = 0;
            Renderer_draw_block(tetris->buffer, tetris->currentBlock);
            Tetris_next_block(tetris);
            int completeLines = Tetris_check_complete_lines(tetris);
            Tetris_check_game_over(tetris);
            Tetris_update_score(tetris, completeLines);
            Tetris_update_level(tetris);
            tetris->buffer->dirty = 1;
        }
    }

    if (tetris->currentKey == KEY_Q
        || tetris->currentKey == KEY_CONTROL_C
    ) {
        Tetris_destroy(tetris);
        exit(0);
    }
    else if (tetris->currentKey == KEY_P) {
        if (tetris->gameState == RUNNING) {
            tetris->gameState = PAUSED;
            Renderer_draw_pause_message(tetris->buffer);
        }
        else if (tetris->gameState == PAUSED) {
            tetris->gameState = RUNNING;
            Renderer_erase_pause_message(tetris->buffer);
        }
    }
    else if (tetris->currentKey == KEY_G) {
        if (tetris->enableGhostBlock) {
            tetris->enableGhostBlock = 0;
        }
        else {
            tetris->enableGhostBlock = 1;
            if (tetris->enableGhostBlock == 1) {
                Tetris_set_ghost_block(tetris);
            }
        }
        tetris->buffer->dirty = 1;
    }


    if (tetris->gameState != PAUSED) {
        if (!Tetris_collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
            tetris->gravityFrameCounter++;
            if (tetris->gravityFrameCounter == tetris->gravityFrameDelay) {
                tetris->gravityFrameCounter = 0;
                tetris->currentBlock->y++;
                tetris->buffer->dirty = 1;
            }
        }
        if (tetris->currentKey == KEY_SPACE) {
            while (!Tetris_collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
                tetris->currentBlock->y++;
                tetris->score += 2;
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_UP && Tetris_rotate_collision(tetris->currentBlock, tetris->buffer) != 1) {
            Block_rotate(tetris->currentBlock);
            if (tetris->enableGhostBlock == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_LEFT
            && !Tetris_collision(LEFT_COLLISION, tetris->currentBlock, tetris->buffer)) {
            tetris->currentBlock->x--;
            if (tetris->enableGhostBlock == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_RIGHT
                && !Tetris_collision(RIGHT_COLLISION, tetris->currentBlock, tetris->buffer)
        ) {
            tetris->currentBlock->x++;
            if (tetris->enableGhostBlock == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_DOWN
                && !Tetris_collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)
        ) {
            tetris->currentBlock->y++;
            if (tetris->enableGhostBlock == 1) {
                Tetris_set_ghost_block(tetris);
            }
            tetris->score++;

//          if (tetris->enableGhostBlock == 1) {
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
        tetris->currentBlock,
        tetris->ghostBlock,
        tetris->colorMode,
        tetris->linesCompleted,
        tetris->score,
        tetris->level,
        tetris->enableGhostBlock
    );
}

int Tetris_check_complete_lines(Tetris *tetris)
{
    int emptyCellCount = 0, lineCount = 0;
    int checkCount, x, y;
    for (checkCount = 0; checkCount < 4; checkCount++) {
        for (y = ROW_FLOOR-1; y > 0; y--) {
            for (x = 1; x < BUFFER_WIDTH-1; x++) {
                if (tetris->buffer->data[y][x] == EMPTY) {
                    emptyCellCount++;
                }
            }
            if (emptyCellCount == 0) {
                lineCount++;
                Tetris_erase_line(tetris->buffer, y);
                tetris->linesCompleted++;
            }
            emptyCellCount = 0;
        }
    }
    tetris->buffer->dirty = 1;
    return lineCount;
}


void Tetris_erase_line(Buffer *buffer, int lineNumber)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        Buffer_set_cell(buffer, i, lineNumber, EMPTY);
    }
    for (i = 1; i < ROW_FLOOR-1; i++) {
        if (lineNumber - i > 0) {
            Tetris_drop_line(buffer, lineNumber - i);
        }
    }
}

void Tetris_drop_line(Buffer *buffer, int lineNumber)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        Buffer_set_cell(buffer, i, lineNumber+1, buffer->data[lineNumber][i]);
        Buffer_set_cell(buffer, i, lineNumber, EMPTY);
    }
}

int Tetris_collision(int Tetris_collisionType, Block *block, Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (Block_get_coord_x(block, Tetris_collisionType, i) == IGNORE
            || Block_get_coord_y(block, Tetris_collisionType, i) == IGNORE
            || block->y + Block_get_coord_y(block, Tetris_collisionType, i) < 0
        ) {
            continue;
        }
        if (Buffer_get_cell(
                buffer,
                block->x + Block_get_coord_x(block, Tetris_collisionType, i),
                block->y + Block_get_coord_y(block, Tetris_collisionType, i)
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
    int originalRotate = block->rotate;
    Block_rotate(block);
    if (Tetris_collision(BOTTOM_COLLISION, block, buffer)
        || Tetris_draw_collision(block, buffer)
    ) {
        Block_set_rotate(block, originalRotate);
        return 1;
    }
    Block_set_rotate(block, originalRotate);
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
    tetris->currentBlock->x = 5;
    tetris->currentBlock->y = 0;
    Block_set_type(tetris->currentBlock, Tetris_get_random_block_type(), 0);

    if (tetris->enableGhostBlock == 1) {
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
    tetris->ghostBlock->x = tetris->currentBlock->x;
    tetris->ghostBlock->y = tetris->currentBlock->y;
    Block_set_type(tetris->ghostBlock, tetris->currentBlock->type, tetris->currentBlock->rotate);
    tetris->ghostBlock->fillType = FILL_GHOST;
    while (!Tetris_collision(BOTTOM_COLLISION, tetris->ghostBlock, tetris->buffer)) {
        tetris->ghostBlock->y++;
    }
}

void Tetris_update_score(Tetris *tetris, int lineCount)
{
    if (lineCount == 1) {
        tetris->score += 100 * tetris->level;
        tetris->linesUntilLevelUp -= 1;
    }
    else if (lineCount == 2) {
        tetris->score += 300 * tetris->level;
        tetris->linesUntilLevelUp -= 3;
    }
    else if (lineCount == 3) {
        tetris->score += 500 * tetris->level;
        tetris->linesUntilLevelUp -= 5;
    }
    else if (lineCount == 4) {
        tetris->score += 800 * tetris->level;
        tetris->linesUntilLevelUp -= 8;
    }
}

void Tetris_update_level(Tetris *tetris)
{
    if (tetris->linesUntilLevelUp <= 0) {
        tetris->linesUntilLevelUp += 10;
        tetris->level++;
        if (tetris->gravityFrameDelay > 1) {
            tetris->gravityFrameDelay--;
        }
    }
}


void Tetris_check_game_over(Tetris *tetris)
{
    if (Tetris_draw_collision(tetris->currentBlock, tetris->buffer)) {
        Renderer_draw_game_over(
            tetris->buffer,
            tetris->colorMode,
            tetris->linesCompleted,
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
