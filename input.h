#ifndef INPUT_H
#define INPUT_H


typedef enum KEY_ALIAS {
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
} KEY_ALIAS;

extern KEY_ALIAS Input_get_key(int *);

#endif
