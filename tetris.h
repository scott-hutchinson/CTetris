#ifndef _tetris_h
#define _tetris_h

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "terminal.h"
#include "input.h"
#include "buffer.h"
#include "block.h"
#include "render.h"

struct Tetris {
    struct Buffer *buffer;
    struct Block *currentBlock, *ghostBlock;
    int currentKey, currentKeySequence [3], blockX, blockY, linesUntilLevelUp;
    uint32_t linesCompleted, score, level;
    uint8_t gameState, bufferDirty, colorMode, enableGhostBlock, gravityFrameCounter, movementFrameCounter, gravityFrameDelay, movementFrameDelay;
};

enum gameStates {
    RUNNING,
    PAUSED,
};

struct Tetris *Tetris_create(void);
void Tetris_destroy(struct Tetris *);

void initScreen(void);
void cleanupScreen(void);

void gameLoop(struct Tetris *);

void getKeyInput(struct Tetris *);
void update(struct Tetris *);
void drawFrame(struct Tetris *);

int checkCompleteLines(struct Tetris *);
void eraseLine(struct Buffer *, int);
void dropLine(struct Buffer *, int);

int collision(int, struct Block *, struct Buffer *);
int rotateCollision(struct Block *, struct Buffer *);
int drawCollision(struct Block *, struct Buffer *);

void nextBlock(struct Tetris *);
int getRandomBlockType(void);
void setGhostBlock(struct Tetris *);

void updateScore(struct Tetris *, int);
void updateLevel(struct Tetris *);
void checkGameOver(struct Tetris *);

#endif
