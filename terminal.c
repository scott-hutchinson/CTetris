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

void Terminal_move_cursor(int direction, int delta)
{
    char direction_code = ((direction == 0) ? 'A' :
                         ((direction == 1) ? 'B' :
                         ((direction == 2) ? 'D' :
                         ((direction == 3) ? 'C' : 0))));

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
