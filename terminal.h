#ifndef _terminal_h
#define _terminal_h

#include <termios.h>


struct termios orig_term_attr, raw_term_attr;

typedef enum {
    XTERM,
    XTERM_256
} COLOR_MODE;

typedef enum {
    CURSOR_UP,
    CURSOR_DOWN,
    CURSOR_RIGHT,
    CURSOR_LEFT,
} CURSOR_DIRECTION;

void Terminal_begin_raw_mode(void);
void Terminal_end_raw_mode(void);

void Terminal_clear_screen(int);

void Terminal_reset_cursor(int);
void Terminal_move_cursor(CURSOR_DIRECTION, int);

void Terminal_set_color(int, int, int);
void Terminal_disable_color(void);

#endif
