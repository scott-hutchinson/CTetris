#ifndef _coordinate_h
#define _coordinate_h

#include <stdint.h>


typedef struct {
    int8_t x, y;
} Coordinate;

Coordinate *Coordinate_create(void);
void Coordinate_destroy(Coordinate *);

typedef enum {
    COORDINATE_MAIN,
    COORDINATE_LEFT_COLLISION,
    COORDINATE_RIGHT_COLLISION,
    COORDINATE_BOTTOM_COLLISION,
    COORDINATE_IGNORE,
} COORDINATE_TYPE;

#endif
