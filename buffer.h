#ifndef _buffer_h
#define _buffer_h

#include <stdlib.h>
#include <stdio.h>

struct Buffer {
    unsigned char **content;
    unsigned char dirty;
};

struct Buffer *Buffer_create(void);
void Buffer_destroy(struct Buffer *);

void fillBuffer(struct Buffer *buffer, unsigned char);
int getCell(struct Buffer *buffer, int, int);
int setCell(struct Buffer *buffer, int, int, unsigned char);

enum config {
    BUFFER_WIDTH  = 12,
    BUFFER_HEIGHT = 24,
    //DIRTY         = 1,
};

#endif
