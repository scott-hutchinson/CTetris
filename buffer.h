#ifndef _buffer_h
#define _buffer_h

#include <stdint.h>


typedef struct {
    uint8_t **data;
    uint8_t dirty;
} Buffer;

Buffer *Buffer_create(void);
void Buffer_destroy(Buffer *);

void fillBuffer(Buffer *buffer, uint8_t);

int getCell(Buffer *buffer, int, int);
int setCell(Buffer *buffer, int, int, uint8_t);

enum config {
    BUFFER_WIDTH  = 12,
    BUFFER_HEIGHT = 24,
};

#endif
