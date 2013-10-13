#ifndef _buffer_h
#define _buffer_h

#include <stdint.h>


typedef struct {
    uint8_t **data;
    uint8_t dirty;
    unsigned int width, height;
} Buffer;

Buffer *Buffer_create(unsigned int, unsigned int);
void Buffer_destroy(Buffer *);

void Buffer_fill(Buffer *, uint8_t);

int Buffer_get_cell(Buffer *, int, int);
int Buffer_set_cell(Buffer *, int, int, uint8_t);

enum config {
    BUFFER_WIDTH  = 12,
    BUFFER_HEIGHT = 24,
};

#endif
