#ifndef _block_h
#define _block_h

#include <stdlib.h>
#include <stdio.h>
#include "coordinate.h"

struct Block {
    unsigned char x, y, type, rotate, fillType;
    struct Coordinate coords [4], leftCollisionCoords [4], rightCollisionCoords [4], bottomCollisionCoords [4];
};

struct Block *Block_create(void);
void Block_destroy(struct Block *);

void setBlockType(struct Block *, unsigned char, unsigned char);
void rotateBlock(struct Block *);
void setBlockRotate(struct Block *, unsigned char);

void clearCollisionCoords(struct Block *);
int getCoordX(struct Block *, int, int);
int getCoordY(struct Block *, int, int);

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
    //FILL_1     = 101,
    //FILL_2     = 102,
    //FILL_3     = 103,
    //FILL_4     = 104,
    //FILL_5     = 105,
    //FILL_6     = 106,
    //FILL_7     = 107,
    //FILL_GHOST = 108,
    FILL_1 = 1,
    FILL_2 = 2,
    FILL_3 = 3,
    FILL_4 = 4,
    FILL_5 = 5,
    FILL_6 = 6,
    FILL_7 = 7,
    FILL_GHOST,
};

#endif
