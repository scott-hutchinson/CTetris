#ifndef _buffer_h
#define _buffer_h

#include <stdint.h>

#include "pixel.h"


typedef struct {
    uint8_t **data;
    uint8_t dirty;
    unsigned int width, height;

    Pixel **pixel_data;
} Buffer;

Buffer *Buffer_create(unsigned int, unsigned int);
void Buffer_destroy(Buffer *);

void Buffer_fill(Buffer *, uint8_t);

int Buffer_get_cell(Buffer *, int, int);
int Buffer_set_cell(Buffer *, int, int, uint8_t);

Pixel *Buffer_get_pixel(Buffer *, unsigned int, unsigned int);
int Buffer_set_pixel(Buffer *, unsigned int, unsigned int, Pixel);

int Buffer_set_pixel_enabled(Buffer *, unsigned int, unsigned int, unsigned char);
int Buffer_set_pixel_bold(Buffer *, unsigned int, unsigned int, unsigned char);
int Buffer_set_pixel_foreground_color(Buffer *, unsigned int, unsigned int, int);
int Buffer_set_pixel_background_color(Buffer *, unsigned int, unsigned int, int);

#endif
