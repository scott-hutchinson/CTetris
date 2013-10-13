#ifndef _input_h
#define _input_h


int Input_get_key(int *);
int Input_get_key_alias(int);

enum key_aliases {
    KEY_NONE,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,
    KEY_Q,
    KEY_P,
    KEY_R,
    KEY_G,
    KEY_SPACE,
    KEY_CONTROL_C,
};

enum key_codes {
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
};

#endif
