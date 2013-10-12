#ifndef _tetris_h
#define _tetris_h

#include <stdint.h>

#include "terminal.h"
#include "input.h"
#include "buffer.h"
#include "block.h"
#include "render.h"


typedef struct {
    Buffer *buffer;
    Block *currentBlock, *ghostBlock;
    int currentKey, currentKeySequence [3], blockX, blockY, linesUntilLevelUp;
    uint32_t linesCompleted, score, level;
    uint8_t gameState, bufferDirty, colorMode, enableGhostBlock, gravityFrameCounter, movementFrameCounter, gravityFrameDelay, movementFrameDelay;
} Tetris;

enum gameStates {
    RUNNING,
    PAUSED,
};

Tetris *Tetris_create(void);
void Tetris_destroy(Tetris *);

void initScreen(void);
void cleanupScreen(void);

void gameLoop(Tetris *);

void getKeyInput(Tetris *);
void update(Tetris *);
void drawFrame(Tetris *);

int checkCompleteLines(Tetris *);
void eraseLine(Buffer *, int);
void dropLine(Buffer *, int);

int collision(int, Block *, Buffer *);
int rotateCollision(Block *, Buffer *);
int drawCollision(Block *, Buffer *);

void nextBlock(Tetris *);
int getRandomBlockType(void);
void setGhostBlock(Tetris *);

void updateScore(Tetris *, int);
void updateLevel(Tetris *);
void checkGameOver(Tetris *);

#endif
