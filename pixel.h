#ifndef _pixel_h
#define _pixel_h


typedef struct Pixel {
    unsigned char enabled, bold;

    int background_color, foreground_color;

    const char *value;
} Pixel;

Pixel *Pixel_create(unsigned char, unsigned char, int, int, const char *);
void Pixel_destroy(Pixel *);

#endif
