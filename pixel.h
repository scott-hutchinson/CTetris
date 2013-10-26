#ifndef _pixel_h
#define _pixel_h


typedef struct {
    unsigned char enabled;
    unsigned char bold;
    int background_color, foreground_color;
    unsigned int value;
} Pixel;

Pixel *Pixel_create(unsigned char, unsigned char, int, int, unsigned int);
void Pixel_destroy(Pixel *);

#endif
