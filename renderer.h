#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include "buffer.h"
#include "block.h"


typedef struct Renderer {
    Buffer *buffer;

    unsigned char color_mode;

    unsigned char color;

    unsigned int left_wall, right_wall;
    unsigned int row_floor, row_line_counter, row_score, row_level;

    char **panel_buffer_line_counter, **panel_buffer_score, **panel_buffer_level;

    unsigned int panel_label_width, panel_value_width;
} Renderer;

typedef enum COLOR_CODE {
    XTERM_GRAY       = 30,
    XTERM_RED        = 31,
    XTERM_GREEN      = 32,
    XTERM_YELLOW     = 33,
    XTERM_BLUE       = 34,
    XTERM_PURPLE     = 35,
    XTERM_CYAN       = 36,
    XTERM_WHITE      = 37,

    XTERM_256_WHITE  = 254,
    XTERM_256_GRAY   = 240,
    XTERM_256_RED    = 34,
    XTERM_256_GREEN  = 112,
    XTERM_256_YELLOW = 226,
    XTERM_256_BLUE   = 27,
    XTERM_256_PURPLE = 129,
    XTERM_256_CYAN   = 45,
    XTERM_256_ORANGE = 166,
} COLOR_CODE;

extern Renderer *Renderer_create(unsigned int, unsigned int, unsigned char);
extern void Renderer_destroy(Renderer *);

extern void Renderer_present_buffer(Renderer *);

extern void Renderer_draw_block(Renderer *, Block *);
extern void Renderer_erase_block(Renderer *, Block *);

extern void Renderer_draw_pause_message(Renderer *);
extern void Renderer_erase_pause_message(Renderer *);

extern void Renderer_draw_panel_labels(Renderer *);
extern void Renderer_erase_panel_labels(Renderer *);

extern void Renderer_draw_panel_line_counter(Renderer *, uint32_t);
extern void Renderer_erase_panel_line_counter(Renderer *);

extern void Renderer_draw_panel_score(Renderer *, uint32_t);
extern void Renderer_erase_panel_score(Renderer *);

extern void Renderer_draw_panel_level(Renderer *, uint32_t);
extern void Renderer_erase_panel_level(Renderer *);

extern void Renderer_draw_game_border(Renderer *);

extern void Renderer_draw_game_over(Renderer *);

#endif
