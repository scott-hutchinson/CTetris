#ifndef RENDERER_H
#define RENDERER_H

#include <stdint.h>

#include "buffer.h"
#include "block.h"


typedef struct Renderer {
    Buffer *buffer;

    int colors[16];

    unsigned int row_floor, row_line_counter, row_score, row_level;
    unsigned int panel_label_width, panel_value_width;

    char **panel_buffer_line_counter, **panel_buffer_score, **panel_buffer_level;
} Renderer;

extern Renderer *Renderer_create(unsigned int, unsigned int);
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
