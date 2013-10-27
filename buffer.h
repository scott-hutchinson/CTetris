#ifndef _buffer_h
#define _buffer_h

#include <stdint.h>

#include "pixel.h"


typedef struct {
    Pixel **pixel_data;

    unsigned int width, height;

    uint8_t dirty;
} Buffer;

Buffer *Buffer_create(unsigned int, unsigned int);
void Buffer_destroy(Buffer *);

void Buffer_fill(Buffer *, uint8_t);

Pixel *Buffer_get_pixel(Buffer *, unsigned int, unsigned int);

unsigned char Buffer_get_pixel_enabled(Buffer *, unsigned int, unsigned int);


int Buffer_set_pixel(Buffer *, unsigned int, unsigned int, Pixel);

int Buffer_set_pixel_enabled(Buffer *, unsigned int, unsigned int, unsigned char);
int Buffer_set_pixel_bold(Buffer *, unsigned int, unsigned int, unsigned char);

int Buffer_set_pixel_foreground_color(Buffer *, unsigned int, unsigned int, int);
int Buffer_set_pixel_background_color(Buffer *, unsigned int, unsigned int, int);

int Buffer_set_pixel_value(Buffer *, unsigned int, unsigned int, const char *);

#endif
