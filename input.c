#include "input.h"

#include <unistd.h>
#include <stdio.h>


KEY_CODE Input_get_key(int *key_sequence) {
    int i;
    for (i = 0; i < 3; i++) {
        key_sequence[i] = ASCII_NULL;
        read(0, &key_sequence[i], 1);
        key_sequence[i] &= 255;
    }

    if (key_sequence[1] == ASCII_NULL
        && key_sequence[2] == ASCII_NULL
    ) {
        return Input_get_key_alias(key_sequence[0]);
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

KEY_ALIAS Input_get_key_alias(int key_code)
{
    switch (key_code) {
        case ASCII_LOWER_Q:
            return KEY_Q;
            break;
        case ASCII_LOWER_P:
            return KEY_P;
            break;
        case ASCII_LOWER_R:
            return KEY_R;
            break;
        case ASCII_LOWER_G:
            return KEY_G;
            break;
        case ASCII_SPACE:
            return KEY_SPACE;
            break;
        case ASCII_CONTROL_C:
            return KEY_CONTROL_C;
            break;
        default:
            return KEY_NONE;
            break;
    }
}
