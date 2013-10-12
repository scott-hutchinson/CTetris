#ifndef _block_h
#define _block_h

#include <stdint.h>

#include "coordinate.h"


typedef struct {
    uint8_t x, y, type, rotate, fillType;
    Coordinate coords [4], leftCollisionCoords [4], rightCollisionCoords [4], bottomCollisionCoords [4];
} Block;

Block *Block_create(void);
void Block_destroy(Block *);

void setBlockType(Block *, uint8_t, uint8_t);
void rotateBlock(Block *);
void setBlockRotate(Block *, uint8_t);

void clearCollisionCoords(Block *);
int getCoordX(Block *, int, int);
int getCoordY(Block *, int, int);

enum blockType {
    BLOCK_I = 1,
    BLOCK_J = 2,
    BLOCK_L = 3,
    BLOCK_O = 4,
    BLOCK_S = 5,
    BLOCK_T = 6,
    BLOCK_Z = 7,
};

enum coordinateTypes {
    MAIN,
    LEFT_COLLISION,
    RIGHT_COLLISION,
    BOTTOM_COLLISION,
    IGNORE,
};

enum fillTypes {
    FILL_1     = 1,
    FILL_2     = 2,
    FILL_3     = 3,
    FILL_4     = 4,
    FILL_5     = 5,
    FILL_6     = 6,
    FILL_7     = 7,
    FILL_GHOST = 8,
};

#endif
