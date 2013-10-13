#ifndef _tetris_h
#define _tetris_h

#include <stdint.h>

#include "terminal.h"
#include "input.h"
#include "buffer.h"
#include "block.h"
#include "renderer.h"


typedef struct {
    Renderer *renderer;
    Buffer *buffer;
    Block *current_block, *ghost_block;
    int current_key, current_key_sequence [3], block_x, block_y, lines_until_level_up;
    uint32_t lines_completed, score, level;
    uint8_t game_state, buffer_dirty, color_mode, enable_ghost_block, gravity_frame_counter, movement_frame_counter, gravity_frame_delay, movement_frame_delay;
} Tetris;

enum game_states {
    RUNNING,
    PAUSED,
};

Tetris *Tetris_create(void);
void Tetris_destroy(Tetris *);

void Tetris_init_screen(void);
void Tetris_cleanup_screen(void);

void Tetris_game_loop(Tetris *);

void Tetris_sleep_ms(unsigned int);
void Tetris_get_key_input(Tetris *);
void Tetris_update(Tetris *);
void Tetris_draw_frame(Tetris *);

int Tetris_check_complete_lines(Tetris *);
void Tetris_erase_line(Buffer *, int);
void Tetris_drop_line(Buffer *, int);

int Tetris_collision(int, Block *, Buffer *);
int Tetris_rotate_collision(Block *, Buffer *);
int Tetris_draw_collision(Block *, Buffer *);

void Tetris_next_block(Tetris *);
int Tetris_get_random_block_type(void);
void Tetris_set_ghost_block(Tetris *);

void Tetris_update_score(Tetris *, int);
void Tetris_update_level(Tetris *);
void Tetris_check_game_over(Tetris *);

#endif
