#ifndef _coordinate_h
#define _coordinate_h


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct Coordinate {
    int8_t x, y;
};

struct Coordinate *Coordinate_create(void);
void Coordinate_destroy(struct Coordinate *);

#endif
