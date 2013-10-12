#include "coordinate.h"

#include <stdlib.h>
#include <stdio.h>


Coordinate *Coordinate_create(void)
{
    Coordinate *block = malloc(sizeof(Coordinate));

    return block;
}

void Coordinate_destroy(Coordinate *block)
{
    free(block);
}

