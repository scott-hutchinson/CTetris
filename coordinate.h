#ifndef _coordinate_h
#define _coordinate_h


#include <stdlib.h>
#include <stdio.h>

struct Coordinate {
    char x, y;
};

struct Coordinate *Coordinate_create(void);
void Coordinate_destroy(struct Coordinate *);

#endif
