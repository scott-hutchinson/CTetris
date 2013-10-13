#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "tetris.h"


Renderer *Renderer_create(unsigned int width, unsigned int height, unsigned char color_mode)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->buffer = Buffer_create(width, height);

    renderer->color_mode = color_mode;



    renderer->left_wall          = 1;
    renderer->right_wall         = width - 2;
    renderer->row_floor          = height - 4;
    renderer->row_line_counter   = height - 3;
    renderer->row_score          = height - 2;
    renderer->row_level          = height - 1;



    return renderer;
}

void Renderer_destroy(Renderer *renderer)
{
    Buffer_destroy(renderer->buffer);

    free(renderer);
}

void Renderer_draw_game(Renderer *renderer, Block *block, Block *ghost_block, unsigned int lines_completed, unsigned int score, unsigned int level, uint8_t enable_ghost_block)
{
    renderer->buffer->dirty = 0;

    if (ghost_block != NULL && enable_ghost_block == 1) {
        Renderer_draw_block(renderer, ghost_block);
    }

    if (block != NULL) {
        Renderer_draw_block(renderer, block);
    }

    int x, y;
    for (y = 0; y < renderer->buffer->height; y++) {
        if (y > renderer->row_floor) {
            Terminal_set_color(XTERM_256_WHITE, 0, 1);
            if (y == renderer->row_line_counter) {
                if (renderer->buffer->data[y][0] == 1) {
                    printf("Lines: %d       ", lines_completed);
                }
                else {
                    printf("        Press P  ");
                }
            }
            else if (y == renderer->row_score) {
                if (renderer->buffer->data[y][0] == 1) {
                    printf("Score: %d                ", score);
                }
                else {
                    printf("      to Continue");
                }
            }
            else if (y == renderer->row_level) {
                if (renderer->buffer->data[y][0] == 1) {
                    printf("Level: %d", level);
                }
                else {
                    printf("                 ");
                }
            }
            Terminal_disable_color();
        }
        else {
            for (x = 0; x < renderer->buffer->width; x++) {
                uint8_t current_cell = renderer->buffer->data[y][x];
                if ((x == 0 || x == renderer->buffer->width-1 || y == renderer->row_floor)
                    && y <= renderer->row_floor
                ) {
                    Terminal_set_color(XTERM_256_GRAY, -1, 1);
                }
                else if (current_cell == FILL_GHOST) {
                    Terminal_set_color(0, 244, 0);
                    //Terminal_set_color(0, XTERM_256_GRAY, 0);
                }
                else if (current_cell == FILL_1) {
                    Terminal_set_color(XTERM_256_CYAN, XTERM_256_CYAN, 0);
                }
                else if (current_cell == FILL_2) {
                    Terminal_set_color(XTERM_256_BLUE, XTERM_256_BLUE, 0);
                }
                else if (current_cell == FILL_3 && renderer->color_mode == XTERM) {
                    Terminal_set_color(XTERM_256_WHITE, XTERM_256_WHITE, 0);
                }
                else if (current_cell == FILL_3 && renderer->color_mode == XTERM_256) {
                    Terminal_set_color(XTERM_256_ORANGE, XTERM_256_ORANGE, 0);
                }
                else if (current_cell == FILL_4) {
                    Terminal_set_color(XTERM_256_YELLOW, XTERM_256_YELLOW, 0);
                }
                else if (current_cell == FILL_5) {
                    Terminal_set_color(XTERM_256_RED, XTERM_256_RED, 0);
                }
                else if (current_cell == FILL_6) {
                    Terminal_set_color(XTERM_256_PURPLE, XTERM_256_PURPLE, 0);
                }
                else if (current_cell == FILL_7) {
                    Terminal_set_color(XTERM_256_GREEN, XTERM_256_GREEN, 0);
                }

                if (current_cell == FILL_WALL) {
                    printf("||");
                }
                else if (current_cell == FILL_FLOOR) {
                    printf("::");
                }
                else if (current_cell == GAMEOVER_0) {
                    printf("GA");
                }
                else if (current_cell == GAMEOVER_1) {
                    printf("ME");
                }
                else if (current_cell == GAMEOVER_2) {
                    printf("OV");
                }
                else if (current_cell == GAMEOVER_3) {
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

    if (ghost_block != NULL && enable_ghost_block == 1) {
        Renderer_erase_block(renderer, ghost_block);
    }
    if (block != NULL) {
        Renderer_erase_block(renderer, block);
    }

    Terminal_clear_screen(0);
}

void Renderer_draw_game_border(Renderer *renderer)
{
    int x, y;
    for (x = 0; x < renderer->buffer->width; x+= renderer->buffer->width-1) {
        for (y = 0; y < renderer->row_floor; y++) {
            Buffer_set_cell(renderer->buffer, x, y, FILL_WALL);
        }
    }
    for (x = 0; x < renderer->buffer->width; x++) {
        Buffer_set_cell(renderer->buffer, x, renderer->row_floor, FILL_FLOOR);
    }
}

void Renderer_draw_game_over(Renderer *renderer, unsigned int lines_completed, unsigned int score, unsigned int level)
{
    renderer->buffer->dirty = 0;
    Renderer_draw_game_border(renderer);

    int x, y;
	for (x = 4, y = 0; y < renderer->row_floor; y++) {
        Tetris_sleep_ms(50);

		Buffer_set_cell(renderer->buffer, x, y, GAMEOVER_0);
		Buffer_set_cell(renderer->buffer, x+1, y, GAMEOVER_1);

        Renderer_draw_game(
            renderer,
            NULL,
            NULL,
            lines_completed,
            score,
            level,
            0
        );

        if (y != renderer->row_floor - 1) {
            Buffer_set_cell(renderer->buffer, x, y, EMPTY);
            Buffer_set_cell(renderer->buffer, x+1, y, EMPTY);
        }

        Terminal_clear_screen(0);
	}

    for (x += 2, y = 0; y < renderer->row_floor; y++) {
        Tetris_sleep_ms(50);

        Buffer_set_cell(renderer->buffer, x, y, GAMEOVER_2);
        Buffer_set_cell(renderer->buffer, x+1, y, GAMEOVER_3);

        Renderer_draw_game(
            renderer,
            NULL,
            NULL,
            lines_completed,
            score,
            level,
            0
        );

        if (y != renderer->row_floor - 1) {
            Buffer_set_cell(renderer->buffer, x, y, EMPTY);
            Buffer_set_cell(renderer->buffer, x+1, y, EMPTY);
        }


        Terminal_clear_screen(0);
    }

	Tetris_sleep_ms(800);
}

void Renderer_draw_block(Renderer *renderer, Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        Buffer_set_cell(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            block->fill_type
        );
    }
}

void Renderer_erase_block(Renderer *renderer, Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        Buffer_set_cell(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            0
        );
    }
}

void Renderer_draw_pause_message(Renderer *renderer)
{
    renderer->buffer->data[renderer->row_line_counter][0] = 0;
    renderer->buffer->data[renderer->row_score][0] = 0;
    renderer->buffer->data[renderer->row_level][0] = 0;
    renderer->buffer->dirty = 1;
}

void Renderer_erase_pause_message(Renderer *renderer)
{
    renderer->buffer->data[renderer->row_line_counter][0] = 1;
    renderer->buffer->data[renderer->row_score][0] = 1;
    renderer->buffer->data[renderer->row_level][0] = 1;
    renderer->buffer->dirty = 1;
}
