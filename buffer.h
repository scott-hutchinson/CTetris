#ifndef _buffer_h
#define _buffer_h

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct Buffer {
    uint8_t **content;
    uint8_t dirty;
};

struct Buffer *Buffer_create(void);
void Buffer_destroy(struct Buffer *);

void fillBuffer(struct Buffer *buffer, uint8_t);
int getCell(struct Buffer *buffer, int, int);
int setCell(struct Buffer *buffer, int, int, uint8_t);

enum config {
    BUFFER_WIDTH  = 12,
    BUFFER_HEIGHT = 24,
    //DIRTY         = 1,
};

#endif
