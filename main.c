#include "tetris.h"


int main(int argc, char **argv)
{
    Tetris *tetris = Tetris_create();

    Tetris_game_loop(tetris);

    Tetris_destroy(tetris);

    return 0;
}
