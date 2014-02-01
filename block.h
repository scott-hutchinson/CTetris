#ifndef BLOCK_H
#define BLOCK_H

#include <stdint.h>


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

typedef enum COORDINATE_TYPE {
    COORDINATE_MAIN,
    COORDINATE_LEFT_COLLISION,
    COORDINATE_RIGHT_COLLISION,
    COORDINATE_BOTTOM_COLLISION,
    COORDINATE_IGNORE,
} COORDINATE_TYPE;

typedef struct Coordinate {
    int8_t x, y;
} Coordinate;

typedef struct Block {
    uint8_t x, y, rotate;

    BLOCK_TYPE type;

    COLOR color;

    Coordinate coords[4];
    Coordinate left_collision_coords[4];
    Coordinate right_collision_coords[4];
    Coordinate bottom_collision_coords[4];
} Block;

extern Block *Block_create(void);
extern void Block_destroy(Block *);

extern void Block_set_type(Block *, BLOCK_TYPE, uint8_t);
extern void Block_rotate(Block *);
extern void Block_set_rotate(Block *, uint8_t);

extern int Block_get_coord_x(Block *, int, int);
extern int Block_get_coord_y(Block *, int, int);

#endif
