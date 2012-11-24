#include "coordinate.h"

struct Coordinate *Coordinate_create(void)
{
    struct Coordinate *block = malloc(sizeof(struct Coordinate));
    return block;
}

void Coordinate_destroy(struct Coordinate *block)
{
    free(block);
}

