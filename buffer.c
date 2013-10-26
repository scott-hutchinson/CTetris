#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"


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


    buffer->pixel_data = malloc(height * sizeof(Pixel *));
    for (y = 0; y < height; y++) {
        buffer->pixel_data[y] = malloc(width * sizeof(Pixel));

    }

    int x;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            buffer->pixel_data[y][x].enabled = 0;
            buffer->pixel_data[y][x].bold = y%2 ^ x%2;
            buffer->pixel_data[y][x].background_color = 0;
            buffer->pixel_data[y][x].foreground_color = 0;
            buffer->pixel_data[y][x].value = 0;
        }
    }

    buffer->width = width;
    buffer->height = height;

    buffer->dirty = 0;

    return buffer;
}

void Buffer_destroy(Buffer *buffer)
{
    int i;
    for(i = 0; i < buffer->height; i++) {
        free(buffer->data[i]);
    }
    free(buffer->data);


    for(i = 0; i < buffer->height; i++) {
        free(buffer->pixel_data[i]);
    }
    free(buffer->pixel_data);


    free(buffer);
}

void Buffer_fill(Buffer *buffer, uint8_t fill_char)
{
    int x, y;
    for (y = 0; y < buffer->height; y++) {
        if (y == buffer->height - 3
            || y == buffer->height - 2
            || y == buffer->height - 1
        ) {
            buffer->data[y][0] = fill_char;
        }
        else {
            for (x = 0; x < buffer->width; x++) {
                buffer->data[y][x] = fill_char;
            }
        }
    }
}

int Buffer_get_cell(Buffer *buffer, int x, int y)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        return buffer->data[y][x];
    }
    return -1;
}

int Buffer_set_cell(Buffer *buffer, int x, int y, uint8_t data)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->data[y][x] = data;
        return 1;
    }
    return 0;
}

Pixel *Buffer_get_pixel(Buffer *buffer, unsigned int x, unsigned int y)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        return &buffer->pixel_data[y][x];
    }

    return 0;
}

int Buffer_set_pixel(Buffer *buffer, unsigned int x, unsigned int y, Pixel pixel)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->pixel_data[y][x] = pixel;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_enabled(Buffer *buffer, unsigned int x, unsigned int y, unsigned char enabled)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->pixel_data[y][x].enabled = enabled;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_bold(Buffer *buffer, unsigned int x, unsigned int y, unsigned char bold)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->pixel_data[y][x].bold = bold;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_foreground_color(Buffer *buffer, unsigned int x, unsigned int y, int foreground_color)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->pixel_data[y][x].foreground_color = foreground_color;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_background_color(Buffer *buffer, unsigned int x, unsigned int y, int background_color)
{
    if (x >= 0 && x < buffer->width && y >= 0 && y < buffer->height) {
        buffer->pixel_data[y][x].background_color = background_color;

        return 1;
    }

    return 0;
}
