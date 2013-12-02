#ifndef _block_h
#define _block_h

#include <stdint.h>

#include "coordinate.h"

typedef enum BLOCK_TYPE {
    BLOCK_NONE,
    BLOCK_I,
    BLOCK_J,
    BLOCK_L,
    BLOCK_O,
    BLOCK_S,
    BLOCK_T,
    BLOCK_Z,
} BLOCK_TYPE;

typedef enum FILL_TYPE {
    FILL_NONE,
    FILL_1,
    FILL_2,
    FILL_3,
    FILL_4,
    FILL_5,
    FILL_6,
    FILL_7,
    FILL_GHOST,
} FILL_TYPE;

typedef enum COLOR {
    COLOR_NONE,
    COLOR_GRAY,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_PURPLE,
    COLOR_CYAN,
    COLOR_WHITE,
    COLOR_ORANGE,
} COLOR;

typedef struct Block {
    uint8_t x, y, rotate;

    BLOCK_TYPE type;

    FILL_TYPE fill_type;

    COLOR color;

    Coordinate coords[4];
    Coordinate left_collision_coords[4];
    Coordinate right_collision_coords[4];
    Coordinate bottom_collision_coords[4];
} Block;

Block *Block_create(void);
void Block_destroy(Block *);

void Block_set_type(Block *, BLOCK_TYPE, uint8_t);
void Block_rotate(Block *);
void Block_set_rotate(Block *, uint8_t);

void Block_clear_collision_coords(Block *);
int Block_get_coord_x(Block *, int, int);
int Block_get_coord_y(Block *, int, int);

#endif
