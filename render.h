#ifndef _render_h
#define _render_h

#include <stdint.h>
#include "buffer.h"
#include "block.h"
#include "terminal.h"
#include "tetris.h"

void drawGame(struct Buffer *, struct Block *, int, unsigned int, unsigned int, unsigned int);
void drawGameBorder(struct Buffer *);
void drawBlock(struct Buffer *, struct Block *);
void eraseBlock(struct Buffer *, struct Block *);
void drawPauseMessage(struct Buffer *);
void erasePauseMessage(struct Buffer *);

enum gameConfig {
    DEFAULT_COLOR_MODE = XTERM_256,
    LEFT_WALL          = 1,
    RIGHT_WALL         = BUFFER_WIDTH - 2,
    ROW_FLOOR          = BUFFER_HEIGHT - 4,
    ROW_LINE_COUNTER   = BUFFER_HEIGHT - 3,
    ROW_SCORE          = BUFFER_HEIGHT - 2,
    ROW_LEVEL          = BUFFER_HEIGHT - 1,
};

enum bufferFillTypes {
    EMPTY,
    FILL_WALL,
    FILL_FLOOR,
    GAMEOVER_0,
    GAMEOVER_1,
    GAMEOVER_2,
    GAMEOVER_3,
    //PAUSED,
};

#endif
