#include "buffer.h"

struct Buffer *Buffer_create(void)
{
    struct Buffer *buffer = malloc(sizeof(struct Buffer));
    buffer->content = malloc(BUFFER_HEIGHT * sizeof(uint8_t *));
    int y;
    for (y = 0; y < BUFFER_HEIGHT; y++) {
        if (y == BUFFER_HEIGHT - 3
            || y == BUFFER_HEIGHT - 2
            || y == BUFFER_HEIGHT - 1
        ) {
            buffer->content[y] = malloc(sizeof(uint8_t));
            buffer->content[y][0] = 0;
        }
        else {
            buffer->content[y] = malloc(BUFFER_WIDTH * sizeof(uint8_t));
        }
    }
    buffer->dirty = 0;

    return buffer;
}

void Buffer_destroy(struct Buffer *buffer)
{
    int i;
    for(i = 0; i < BUFFER_HEIGHT; i++) {
        free(buffer->content[i]);
    }
    free(buffer->content);
    free(buffer);
}

void fillBuffer(struct Buffer *buffer, uint8_t fillChar)
{
    int x, y;
    for (y = 0; y < BUFFER_HEIGHT; y++) {
        if (y == BUFFER_HEIGHT - 3
            || y == BUFFER_HEIGHT - 2
            || y == BUFFER_HEIGHT - 1
        ) {
            buffer->content[y][0] = fillChar;
        }
        else {
            for (x = 0; x < BUFFER_WIDTH; x++) {
                buffer->content[y][x] = fillChar;
            }
        }
    }
}

int getCell(struct Buffer *buffer, int x, int y)
{
    if(x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        return buffer->content[y][x];
    }
    return -1;
}

int setCell(struct Buffer *buffer, int x, int y, uint8_t content)
{
    if(x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT) {
        buffer->content[y][x] = content;
        return 1;
    }
    return 0;
}
