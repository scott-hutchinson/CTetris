#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

#include "pixel.h"


typedef struct Buffer {
    Pixel **pixel_data;

    unsigned int width, height;

    uint8_t dirty;
} Buffer;

extern Buffer *Buffer_create(unsigned int, unsigned int);
extern void Buffer_destroy(Buffer *);

extern void Buffer_fill(Buffer *, uint8_t);

extern Pixel *Buffer_get_pixel(Buffer *, unsigned int, unsigned int);

extern unsigned char Buffer_get_pixel_enabled(Buffer *, unsigned int, unsigned int);


extern int Buffer_set_pixel(Buffer *, unsigned int, unsigned int, Pixel);

extern int Buffer_set_pixel_enabled(Buffer *, unsigned int, unsigned int, unsigned char);
extern int Buffer_set_pixel_bold(Buffer *, unsigned int, unsigned int, unsigned char);

extern int Buffer_set_pixel_foreground_color(Buffer *, unsigned int, unsigned int, int);
extern int Buffer_set_pixel_background_color(Buffer *, unsigned int, unsigned int, int);

extern int Buffer_set_pixel_value(Buffer *, unsigned int, unsigned int, const char *);

#endif
