#ifndef _tetris_h
#define _tetris_h

#include <stdint.h>

#include "terminal.h"
#include "input.h"
#include "block.h"
#include "renderer.h"


typedef struct {
    Renderer *renderer;

    Block *current_block, *ghost_block;

    uint32_t score, level, lines_until_level_up, lines_completed;

    uint8_t game_state, enable_ghost_block;

    uint8_t gravity_frame_counter, movement_frame_counter;
    uint8_t gravity_frame_delay, movement_frame_delay;

    int current_key, current_key_sequence[3];

    unsigned char key_left, key_right, key_down, key_drop, key_rotate, key_quit, key_pause, key_ghost_block;
} Tetris;

typedef enum GAME_STATE {
    RUNNING,
    PAUSED,
} GAME_STATE;

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
void Tetris_erase_line(Renderer *, int);
void Tetris_drop_line(Buffer *, int);

int Tetris_collision(int, Block *, Buffer *);
int Tetris_rotate_collision(Block *, Buffer *);
int Tetris_draw_collision(Block *, Buffer *);

void Tetris_next_block(Tetris *);
int Tetris_get_random_block_type(void);
int random_in_range(unsigned int, unsigned int);
void Tetris_set_ghost_block(Tetris *);

void Tetris_update_score(Tetris *, int);
void Tetris_update_level(Tetris *);
void Tetris_check_game_over(Tetris *);

#endif
