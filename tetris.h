#ifndef TETRIS_H
#define TETRIS_H


typedef enum GAME_STATE {
    RUNNING,
    PAUSED,
} GAME_STATE;

typedef struct Tetris Tetris;

extern Tetris *Tetris_create(void);
extern void Tetris_destroy(Tetris *);

extern void Tetris_init_options(Tetris *, int, char **);

extern void Tetris_game_loop(Tetris *);

extern void Tetris_sleep_ms(unsigned int milliseconds);

#endif
