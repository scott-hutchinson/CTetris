#include "input.h"

#include <unistd.h>

typedef enum KEY_CODE {
    ASCII_NULL                = 0x00,
    ASCII_LOWER_Q             = 0x71,
    ASCII_LOWER_P             = 0x70,
    ASCII_LOWER_R             = 0x72,
    ASCII_LOWER_G             = 0x67,
    ASCII_UPPER_A             = 0x41,
    ASCII_UPPER_B             = 0x42,
    ASCII_UPPER_C             = 0x43,
    ASCII_UPPER_D             = 0x44,
    ASCII_SPACE               = 0x20,
    ASCII_ESCAPE              = 0x1B,
    ASCII_SQUARE_BRACKET_LEFT = 0x5B,
    ASCII_CONTROL_C           = 0x03,
} KEY_CODE;


static KEY_ALIAS get_key_alias(int key_code)
{
    switch (key_code) {
        case ASCII_LOWER_Q:
            return KEY_Q;

        case ASCII_LOWER_P:
            return KEY_P;

        case ASCII_LOWER_R:
            return KEY_R;

        case ASCII_LOWER_G:
            return KEY_G;

        case ASCII_SPACE:
            return KEY_SPACE;

        case ASCII_CONTROL_C:
            return KEY_CONTROL_C;

        default:
            return KEY_NONE;
    }
}

KEY_ALIAS Input_get_key(int *key_sequence) {
    int i;
    for (i = 0; i < 3; ++i) {
        key_sequence[i] = ASCII_NULL;
        read(0, &key_sequence[i], 1);
        key_sequence[i] &= 255;
    }

    if (key_sequence[1] == ASCII_NULL
        && key_sequence[2] == ASCII_NULL
    ) {
        return get_key_alias(key_sequence[0]);
    }
    else if (key_sequence[0] == ASCII_ESCAPE
             && key_sequence[1] == ASCII_SQUARE_BRACKET_LEFT
    ) {
        if (key_sequence[2] == ASCII_UPPER_D) {
            return KEY_LEFT;
        }
        else if (key_sequence[2] == ASCII_UPPER_C) {
            return KEY_RIGHT;
        }
        else if (key_sequence[2] == ASCII_UPPER_A) {
            return KEY_UP;
        }
        else if (key_sequence[2] == ASCII_UPPER_B) {
            return KEY_DOWN;
        }
    }

    return KEY_NONE;
}
