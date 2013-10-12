#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "tetris.h"


Renderer *Renderer_create(unsigned int width, unsigned int height, unsigned char color_mode)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->buffer = Buffer_create(width, width);

    renderer->color_mode = color_mode;

    return renderer;
}

void Renderer_destroy(Renderer *renderer)
{
    Buffer_destroy(renderer->buffer);

    free(renderer);
}

void Renderer_draw_game(Buffer *buffer, Block *block, Block *ghostBlock, int colorMode, unsigned int linesCompleted, unsigned int score, unsigned int level, uint8_t enableGhostBlock)
{
    buffer->dirty = 0;

    if (ghostBlock != NULL && enableGhostBlock == 1) {
        Renderer_draw_block(buffer, ghostBlock);
    }

    if (block != NULL) {
        Renderer_draw_block(buffer, block);
    }

    int x, y;
    for (y = 0; y < BUFFER_HEIGHT; y++) {
        if (y > ROW_FLOOR) {
            Terminal_set_color(XTERM, WHITE, 0, 1);
            if (y == ROW_LINE_COUNTER) {
                if (buffer->data[y][0] == 1) {
                    printf("Lines: %d       ", linesCompleted);
                }
                else {
                    printf("        Press P  ");
                }
            }
            else if (y == ROW_SCORE) {
                if (buffer->data[y][0] == 1) {
                    printf("Score: %d                ", score);
                }
                else {
                    printf("      to Continue");
                }
            }
            else if (y == ROW_LEVEL) {
                if (buffer->data[y][0] == 1) {
                    printf("Level: %d", level);
                }
                else {
                    printf("                 ");
                }
            }
            Terminal_disable_color();
        }
        else {
            for (x = 0; x < BUFFER_WIDTH; x++) {
                uint8_t currentCell = buffer->data[y][x];
                if ((x == 0 || x == BUFFER_WIDTH-1 || y == ROW_FLOOR)
                    && y <= ROW_FLOOR
                ) {
                    Terminal_set_color(XTERM_256, GRAY, 0, 1);
                }
                else if (currentCell == FILL_GHOST) {
                    Terminal_set_color(XTERM_256, 0, 244, 0);
                    //Terminal_set_color(XTERM_256, 0, GRAY, 0);
                }
                else if (currentCell == FILL_1) {
                    Terminal_set_color(XTERM_256, CYAN, CYAN, 0);
                }
                else if (currentCell == FILL_2) {
                    Terminal_set_color(XTERM_256, BLUE, BLUE, 0);
                }
                else if (currentCell == FILL_3 && colorMode == XTERM) {
                    Terminal_set_color(XTERM, WHITE, WHITE, 0);
                }
                else if (currentCell == FILL_3 && colorMode == XTERM_256) {
                    Terminal_set_color(XTERM_256, ORANGE, ORANGE, 0);
                }
                else if (currentCell == FILL_4) {
                    Terminal_set_color(XTERM_256, YELLOW, YELLOW, 0);
                }
                else if (currentCell == FILL_5) {
                    Terminal_set_color(XTERM_256, RED, RED, 0);
                }
                else if (currentCell == FILL_6) {
                    Terminal_set_color(XTERM_256, PURPLE, PURPLE, 0);
                }
                else if (currentCell == FILL_7) {
                    Terminal_set_color(XTERM_256, GREEN, GREEN, 0);
                }

                if (currentCell == FILL_WALL) {
                    printf("||");
                }
                else if (currentCell == FILL_FLOOR) {
                    printf("::");
                }
                else if (currentCell == GAMEOVER_0) {
                    printf("GA");
                }
                else if (currentCell == GAMEOVER_1) {
                    printf("ME");
                }
                else if (currentCell == GAMEOVER_2) {
                    printf("OV");
                }
                else if (currentCell == GAMEOVER_3) {
                    printf("ER");
                }
                else {
                    printf("  ");
                }
                Terminal_disable_color();
            }
        }
        printf("\n");

    }

    if (ghostBlock != NULL && enableGhostBlock == 1) {
        Renderer_erase_block(buffer, ghostBlock);
    }
    if (block != NULL) {
        Renderer_erase_block(buffer, block);
    }

    Terminal_clear_screen(0);
}

void Renderer_draw_game_border(Buffer *buffer)
{
    int x, y;
    for (x = 0; x < BUFFER_WIDTH; x+= BUFFER_WIDTH-1) {
        for (y = 0; y < ROW_FLOOR; y++) {
            Buffer_set_cell(buffer, x, y, FILL_WALL);
        }
    }
    for (x = 0; x < BUFFER_WIDTH; x++) {
        Buffer_set_cell(buffer, x, ROW_FLOOR, FILL_FLOOR);
    }
}

void Renderer_draw_game_over(Buffer *buffer, int colorMode, unsigned int linesCompleted, unsigned int score, unsigned int level)
{
    buffer->dirty = 0;
    Renderer_draw_game_border(buffer);

    int x, y;
	for (x = 4, y = 0; y < ROW_FLOOR; y++) {
        Tetris_sleep_ms(50);

		Buffer_set_cell(buffer, x, y, GAMEOVER_0);
		Buffer_set_cell(buffer, x+1, y, GAMEOVER_1);

        Renderer_draw_game(
            buffer,
            NULL,
            NULL,
            colorMode,
            linesCompleted,
            score,
            level,
            0
        );

        if (y != ROW_FLOOR - 1) {
            Buffer_set_cell(buffer, x, y, EMPTY);
            Buffer_set_cell(buffer, x+1, y, EMPTY);
        }

        Terminal_clear_screen(0);
	}

    for (x += 2, y = 0; y < ROW_FLOOR; y++) {
        Tetris_sleep_ms(50);

        Buffer_set_cell(buffer, x, y, GAMEOVER_2);
        Buffer_set_cell(buffer, x+1, y, GAMEOVER_3);

        Renderer_draw_game(
            buffer,
            NULL,
            NULL,
            colorMode,
            linesCompleted,
            score,
            level,
            0
        );

        if (y != ROW_FLOOR - 1) {
            Buffer_set_cell(buffer, x, y, EMPTY);
            Buffer_set_cell(buffer, x+1, y, EMPTY);
        }


        Terminal_clear_screen(0);
    }

	Tetris_sleep_ms(800);
}


void Renderer_draw_block(Buffer *buffer, Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        Buffer_set_cell(
            buffer,
            block->x + Block_get_coord_x(block, MAIN, i),
            block->y + Block_get_coord_y(block, MAIN, i),
            block->fillType
        );
    }
}

void Renderer_erase_block(Buffer *buffer, Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        Buffer_set_cell(
            buffer,
            block->x + Block_get_coord_x(block, MAIN, i),
            block->y + Block_get_coord_y(block, MAIN, i),
            0
        );
    }
}

void Renderer_draw_pause_message(Buffer *buffer)
{
    buffer->data[ROW_LINE_COUNTER][0] = 0;
    buffer->data[ROW_SCORE][0] = 0;
    buffer->data[ROW_LEVEL][0] = 0;
    buffer->dirty = 1;
}

void Renderer_erase_pause_message(Buffer *buffer)
{
    buffer->data[ROW_LINE_COUNTER][0] = 1;
    buffer->data[ROW_SCORE][0] = 1;
    buffer->data[ROW_LEVEL][0] = 1;
    buffer->dirty = 1;
}
