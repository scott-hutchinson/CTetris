#include "tetris.h"

int main(int argc, char **argv)
{
    struct Tetris *tetris = Tetris_create();
    gameLoop(tetris);
    Tetris_destroy(tetris);

    return 0;
}
