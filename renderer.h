#ifndef _renderer_h
#define _renderer_h

#include <stdint.h>

#include "buffer.h"
#include "block.h"


typedef struct {
    Buffer *buffer;
    unsigned char color_mode;
} Renderer;

Renderer *Renderer_create(unsigned int, unsigned int, unsigned char);
void Renderer_destroy(Renderer *);

void Renderer_draw_game(Buffer *, Block *, Block *, int, unsigned int, unsigned int, unsigned int, uint8_t);
void Renderer_draw_game_border(Buffer *);
void Renderer_draw_game_over(Buffer *, int, unsigned int, unsigned int, unsigned int);
void Renderer_draw_block(Buffer *, Block *);
void Renderer_erase_block(Buffer *, Block *);
void Renderer_draw_pause_message(Buffer *);
void Renderer_erase_pause_message(Buffer *);

enum game_config {
    LEFT_WALL          = 1,
    RIGHT_WALL         = BUFFER_WIDTH - 2,
    ROW_FLOOR          = BUFFER_HEIGHT - 4,
    ROW_LINE_COUNTER   = BUFFER_HEIGHT - 3,
    ROW_SCORE          = BUFFER_HEIGHT - 2,
    ROW_LEVEL          = BUFFER_HEIGHT - 1,
};

enum buffer_fill_types {
    EMPTY,
    FILL_WALL,
    FILL_FLOOR,
    GAMEOVER_0 = 101,
    GAMEOVER_1 = 102,
    GAMEOVER_2 = 103,
    GAMEOVER_3 = 104,
};

#endif
