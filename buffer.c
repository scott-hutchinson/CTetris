#include "buffer.h"

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"


Buffer *Buffer_create(unsigned int width, unsigned int height)
{
    Buffer *buffer = malloc(sizeof(Buffer));

    unsigned int y;
    buffer->pixel_data = malloc(height * sizeof(Pixel *));
    for (y = 0; y < height; y++) {
        buffer->pixel_data[y] = malloc(width * sizeof(Pixel));
    }

    unsigned int x;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            buffer->pixel_data[y][x].enabled = 0;

            buffer->pixel_data[y][x].bold = 0;

            buffer->pixel_data[y][x].background_color = 0;
            buffer->pixel_data[y][x].foreground_color = 0;

            buffer->pixel_data[y][x].value = "  ";
        }
    }

    buffer->width = width;
    buffer->height = height;

    buffer->dirty = 0;

    return buffer;
}

void Buffer_destroy(Buffer *buffer)
{
    unsigned int i;
    for(i = 0; i < buffer->height; i++) {
        free(buffer->pixel_data[i]);
    }
    free(buffer->pixel_data);

    free(buffer);
}

Pixel *Buffer_get_pixel(Buffer *buffer, unsigned int x, unsigned int y)
{
    if (x < buffer->width && y < buffer->height) {
        return &buffer->pixel_data[y][x];
    }

    return 0;
}

unsigned char Buffer_get_pixel_enabled(Buffer *buffer, unsigned int x, unsigned int y)
{
    if (x < buffer->width && y < buffer->height) {
        return buffer->pixel_data[y][x].enabled;
    }

    return 0;
}

int Buffer_set_pixel(Buffer *buffer, unsigned int x, unsigned int y, Pixel pixel)
{
    if (x < buffer->width && y < buffer->height) {
        buffer->pixel_data[y][x] = pixel;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_enabled(Buffer *buffer, unsigned int x, unsigned int y, unsigned char enabled)
{
    if (x < buffer->width && y < buffer->height) {
        buffer->pixel_data[y][x].enabled = enabled;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_bold(Buffer *buffer, unsigned int x, unsigned int y, unsigned char bold)
{
    if (x < buffer->width && y < buffer->height) {
        buffer->pixel_data[y][x].bold = bold;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_foreground_color(Buffer *buffer, unsigned int x, unsigned int y, int foreground_color)
{
    if (x < buffer->width && y < buffer->height) {
        buffer->pixel_data[y][x].foreground_color = foreground_color;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_background_color(Buffer *buffer, unsigned int x, unsigned int y, int background_color)
{
    if (x < buffer->width && y < buffer->height) {
        buffer->pixel_data[y][x].background_color = background_color;

        return 1;
    }

    return 0;
}

int Buffer_set_pixel_value(Buffer *buffer, unsigned int x, unsigned int y, const char *value)
{
    if ((x < buffer->width && y < buffer->height)
        && value != NULL
    ) {
        buffer->pixel_data[y][x].value = value;

        return 1;
    }

    return 0;
}
