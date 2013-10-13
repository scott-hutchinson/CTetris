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

void Terminal_set_color(COLOR_MODE color_mode, COLOR_CODE text_color, COLOR_CODE background_color, int bold)
{
    printf("\033[");

    if (bold == 1) {
        printf("1");
    }

    if (color_mode == XTERM) {
        switch (text_color) {
            case GRAY:
                printf(";%d", XTERM_GRAY);
                break;
            case RED:
                printf(";%d", XTERM_RED);
                break;
            case GREEN:
                printf(";%d", XTERM_GREEN);
                break;
            case YELLOW:
                printf(";%d", XTERM_YELLOW);
                break;
            case BLUE:
                printf(";%d", XTERM_BLUE);
                break;
            case PURPLE:
                printf(";%d", XTERM_PURPLE);
                break;
            case CYAN:
                printf(";%d", XTERM_CYAN);
                break;
            case WHITE:
                printf(";%d", XTERM_WHITE);
                break;
            case NONE:
                break;
            default:
                printf(";%d", text_color);
                break;
        }

        switch (background_color) {
            case GRAY:
                printf(";%d", XTERM_GRAY + 10);
                break;
            case RED:
                printf(";%d", XTERM_RED + 10);
                break;
            case GREEN:
                printf(";%d", XTERM_GREEN + 10);
                break;
            case YELLOW:
                printf(";%d", XTERM_YELLOW + 10);
                break;
            case BLUE:
                printf(";%d", XTERM_BLUE + 10);
                break;
            case PURPLE:
                printf(";%d", XTERM_PURPLE + 10);
                break;
            case CYAN:
                printf(";%d", XTERM_CYAN + 10);
                break;
            case WHITE:
                printf(";%d", XTERM_WHITE + 10);
                break;
            case NONE:
                break;
            default:
                printf(";%d", background_color);
                break;
        }
    }
    else if (color_mode == XTERM_256) {
        switch (text_color) {
            case GRAY:
                printf(";38;5;%d", XTERM_256_GRAY);
                break;
            case RED:
                printf(";38;5;%d", XTERM_256_RED);
                break;
            case GREEN:
                printf(";38;5;%d", XTERM_256_GREEN);
                break;
            case YELLOW:
                printf(";38;5;%d", XTERM_256_YELLOW);
                break;
            case BLUE:
                printf(";38;5;%d", XTERM_256_BLUE);
                break;
            case PURPLE:
                printf(";38;5;%d", XTERM_256_PURPLE);
                break;
            case CYAN:
                printf(";38;5;%d", XTERM_256_CYAN);
                break;
            case WHITE:
                printf(";38;5;%d", XTERM_256_ORANGE);
                break;
            case NONE:
                break;
            default:
                printf(";38;5;%d", text_color);
                break;
        }

        switch (background_color) {
            case GRAY:
                printf(";48;5;%d", XTERM_256_GRAY);
                break;
            case RED:
                printf(";48;5;%d", XTERM_256_RED);
                break;
            case GREEN:
                printf(";48;5;%d", XTERM_256_GREEN);
                break;
            case YELLOW:
                printf(";48;5;%d", XTERM_256_YELLOW);
                break;
            case BLUE:
                printf(";48;5;%d", XTERM_256_BLUE);
                break;
            case PURPLE:
                printf(";48;5;%d", XTERM_256_PURPLE);
                break;
            case CYAN:
                printf(";48;5;%d", XTERM_256_CYAN);
                break;
            case ORANGE:
                printf(";48;5;%d", XTERM_256_ORANGE);
                break;
            case NONE:
                break;
            default:
                printf(";48;5;%d", background_color);
                break;
        }
    }

    printf("m");
}

void Terminal_disable_color(void)
{
    printf("\033[0m");
}
