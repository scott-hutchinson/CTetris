#ifndef TETRIS_H
#define TETRIS_H

#include <stdint.h>

#include "block.h"
#include "renderer.h"


typedef enum GAME_STATE {
    RUNNING,
    PAUSED,
} GAME_STATE;

typedef struct Tetris {
    Renderer *renderer;

    Block *current_block, *ghost_block;

    GAME_STATE game_state;

    uint32_t score, level, lines_until_level_up, lines_completed;

    uint8_t enable_ghost_block;

    uint8_t gravity_frame_counter, movement_frame_counter;
    uint8_t gravity_frame_delay, movement_frame_delay;

    int current_key, current_key_sequence[3];

    unsigned char key_left, key_right, key_down, key_drop, key_rotate, key_quit, key_pause, key_ghost_block;

    int option_count;
    char **options;
} Tetris;

extern Tetris *Tetris_create(void);
extern void Tetris_destroy(Tetris *);

extern void Tetris_init_options(Tetris *, int, char **);

extern void Tetris_game_loop(Tetris *);

extern void Tetris_sleep_ms(unsigned int milliseconds);

#endif
