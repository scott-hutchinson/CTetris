#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>


Buffer *Buffer_create(unsigned int width, unsigned int height)
{
    Buffer *buffer = malloc(sizeof(Buffer));
    buffer->data = malloc(height * sizeof(uint8_t *));
    int y;
    for (y = 0; y < height; y++) {
        if (y == height - 3
            || y == height - 2
            || y == height - 1
        ) {
            buffer->data[y] = malloc(sizeof(uint8_t));
            buffer->data[y][0] = 0;
        }
        else {
            buffer->data[y] = malloc(width * sizeof(uint8_t));
        }
    }
    buffer->dirty = 0;

    return buffer;
}

void Buffer_destroy(Buffer *buffer)
{
    int i;
    for(i = 0; i < BUFFER_HEIGHT; i++) {
        free(buffer->data[i]);
    }
    free(buffer->data);
    free(buffer);
}

void Buffer_fill(Buffer *buffer, uint8_t fillChar)
{
    int x, y;
    for (y = 0; y < BUFFER_HEIGHT; y++) {
        if (y == BUFFER_HEIGHT - 3
            || y == BUFFER_HEIGHT - 2
            || y == BUFFER_HEIGHT - 1
        ) {
            buffer->data[y][0] = fillChar;
        }
        else {
            for (x = 0; x < BUFFER_WIDTH; x++) {
                buffer->data[y][x] = fillChar;
            }
        }
    }
}

int Buffer_get_cell(Buffer *buffer, int x, int y)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        return buffer->data[y][x];
    }
    return -1;
}

int Buffer_set_cell(Buffer *buffer, int x, int y, uint8_t data)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        buffer->data[y][x] = data;
        return 1;
    }
    return 0;
}
