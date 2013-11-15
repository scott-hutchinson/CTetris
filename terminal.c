#include "terminal.h"

#include <stdio.h>
#include <stdlib.h>


void Terminal_begin_raw_mode(void)
{
    tcgetattr(fileno(stdin), &orig_term_attr);

    raw_term_attr = orig_term_attr;

    raw_term_attr.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw_term_attr.c_cc[VTIME] = 0;
    raw_term_attr.c_cc[VMIN] = 0;

    tcsetattr(fileno(stdin), TCSANOW, &raw_term_attr);
}

void Terminal_end_raw_mode(void)
{
    tcsetattr(fileno(stdin), TCSANOW, &orig_term_attr);
}

void Terminal_clear_screen(int mode)
{
    if (mode == 0) {
        printf("\033[H\033[1J");
    }
    else if (mode == 1) {
        printf("\033[H\033[2J");
    }
}

void Terminal_set_cursor(int mode)
{
    if (mode == 0) {
        printf("\033[?25l");
    }
    else if (mode == 1) {
        printf("\033[?25h");
    }
}

void Terminal_move_cursor(CURSOR_DIRECTION direction, int delta)
{
    char direction_code;

    switch (direction) {
        case CURSOR_UP:
            direction_code = 'A';
            break;

        case CURSOR_DOWN:
            direction_code = 'B';
            break;

        case CURSOR_RIGHT:
            direction_code = 'C';
            break;

        case CURSOR_LEFT:
            direction_code = 'D';
            break;

        default:
            direction_code = 0;
            break;
    }

    if (direction_code != 0) {
        printf("\033[%d%c", delta, direction_code);
    }
}

void Terminal_set_color(int text_color, int background_color, int bold)
{
    printf("\033[");

    if (bold == 1) {
        printf("1");
    }

    if (text_color > 0) {
        printf(";38;5;%d", text_color);
    }

    if (background_color > 0) {
        printf(";48;5;%d", background_color);
    }

    printf("m");
}

void Terminal_disable_color(void)
{
    printf("\033[0m");
}
