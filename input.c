#include "input.h"

int getKey(int *keySequence) {
    int i;
    for (i = 0; i < 3; i++) {
        keySequence[i] = ASCII_NULL;
        read(0, &keySequence[i], 1);
        keySequence[i] &= 255;
    }
    if (keySequence[1] == ASCII_NULL
        && keySequence[2] == ASCII_NULL
    ) {
        return getKeyAlias(keySequence[0]);
    }
    else if (keySequence[0] == ASCII_ESCAPE
            && keySequence[1] == ASCII_SQUARE_BRACKET_LEFT
    ) {
        if (keySequence[2] == ASCII_UPPER_D) {
            return KEY_LEFT;
        }
        else if (keySequence[2] == ASCII_UPPER_C) {
            return KEY_RIGHT;
        }
        else if (keySequence[2] == ASCII_UPPER_A) {
            return KEY_UP;
        }
        else if (keySequence[2] == ASCII_UPPER_B) {
            return KEY_DOWN;
        }
    }
    return 0;
}

int getKeyAlias(int keyCode)
{
    switch (keyCode) {
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
