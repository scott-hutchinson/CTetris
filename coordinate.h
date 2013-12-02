#ifndef _coordinate_h
#define _coordinate_h

#include <stdint.h>


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

extern Coordinate *Coordinate_create(void);
extern void Coordinate_destroy(Coordinate *);

#endif
