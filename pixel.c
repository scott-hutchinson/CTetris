#include "pixel.h"

#include <stdlib.h>


Pixel *Pixel_create(unsigned char enabled, unsigned char bold, int background_color, int foreground_color, const char *value)
{
    Pixel *pixel = malloc(sizeof(Pixel));

    pixel->enabled = enabled;

    pixel->bold = bold;

    pixel->background_color = background_color;
    pixel->foreground_color = foreground_color;

    pixel->value = value;

    return pixel;
}

void Pixel_destroy(Pixel *pixel)
{
    free(pixel);
}
