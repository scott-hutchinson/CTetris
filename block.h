#ifndef _block_h
#define _block_h

#include <stdint.h>

#include "coordinate.h"


typedef struct {
    uint8_t x, y, type, rotate, fill_type;
    Coordinate coords [4];
    Coordinate left_collision_coords [4];
    Coordinate right_collision_coords [4];
    Coordinate bottom_collision_coords [4];
} Block;

Block *Block_create(void);
void Block_destroy(Block *);

void Block_set_type(Block *, uint8_t, uint8_t);
void Block_rotate(Block *);
void Block_set_rotate(Block *, uint8_t);

void Block_clear_collision_coords(Block *);
int Block_get_coord_x(Block *, int, int);
int Block_get_coord_y(Block *, int, int);

enum block_type {
    BLOCK_I = 1,
    BLOCK_J = 2,
    BLOCK_L = 3,
    BLOCK_O = 4,
    BLOCK_S = 5,
    BLOCK_T = 6,
    BLOCK_Z = 7,
};

enum coordinate_types {
    MAIN,
    LEFT_COLLISION,
    RIGHT_COLLISION,
    BOTTOM_COLLISION,
    IGNORE,
};

enum fill_types {
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
