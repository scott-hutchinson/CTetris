#ifndef _coordinate_h
#define _coordinate_h

#include <stdint.h>


typedef struct {
    int8_t x, y;
} Coordinate;

Coordinate *Coordinate_create(void);
void Coordinate_destroy(Coordinate *);

#endif
