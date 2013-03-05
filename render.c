#include "render.h"


void drawGame(struct Buffer *buffer, struct Block *block, struct Block *ghostBlock, int colorMode, unsigned int linesCompleted, unsigned int score, unsigned int level, uint8_t enableGhostBlock)
{
    buffer->dirty = 0;
    if (enableGhostBlock == 1) {
        drawBlock(buffer, ghostBlock);
    }
    drawBlock(buffer, block);

    int x, y;
    for (y = 0; y < BUFFER_HEIGHT; y++) {
        if (y > ROW_FLOOR) {
            setColor(XTERM, WHITE, 0, 1);
            if (y == ROW_LINE_COUNTER) {
                if (buffer->content[y][0] == 1) {
                    printf("Lines: %d       ", linesCompleted);
                }
                else {
                    printf("        Press P  ");
                }
            }
            else if (y == ROW_SCORE) {
                if (buffer->content[y][0] == 1) {
                    printf("Score: %d                ", score);
                }
                else {
                    printf("      to Continue");
                }
            }
            else if (y == ROW_LEVEL) {
                if (buffer->content[y][0] == 1) {
                    printf("Level: %d", level);
                }
                else {
                    printf("                 ");
                }
            }
            disableColor();
        }
        else {
            for (x = 0; x < BUFFER_WIDTH; x++) {
                uint8_t currentCell = buffer->content[y][x];
                if ((x == 0 || x == BUFFER_WIDTH-1 || y == ROW_FLOOR)
                    && y <= ROW_FLOOR
                ) {
                    setColor(XTERM_256, GRAY, 0, 1);
                }
                else if (currentCell == FILL_GHOST) {
                    setColor(XTERM_256, 0, 244, 0);
                    //setColor(XTERM_256, 0, GRAY, 0);
                }
                else if (currentCell == FILL_1) {
                    setColor(XTERM_256, CYAN, CYAN, 0);
                }
                else if (currentCell == FILL_2) {
                    setColor(XTERM_256, BLUE, BLUE, 0);
                }
                else if (currentCell == FILL_3 && colorMode == XTERM) {
                    setColor(XTERM, WHITE, WHITE, 0);
                }
                else if (currentCell == FILL_3 && colorMode == XTERM_256) {
                    setColor(XTERM_256, ORANGE, ORANGE, 0);
                }
                else if (currentCell == FILL_4) {
                    setColor(XTERM_256, YELLOW, YELLOW, 0);
                }
                else if (currentCell == FILL_5) {
                    setColor(XTERM_256, RED, RED, 0);
                }
                else if (currentCell == FILL_6) {
                    setColor(XTERM_256, PURPLE, PURPLE, 0);
                }
                else if (currentCell == FILL_7) {
                    setColor(XTERM_256, GREEN, GREEN, 0);
                }

                if (currentCell == FILL_WALL) {
                    printf("||");
                }
                else if (currentCell == FILL_FLOOR) {
                    printf("::");
                }
                // else if (currentCell == GAMEOVER_0) {
                    // printf("GA");
                // }
                // else if (currentCell == GAMEOVER_1) {
                    // printf("ME");
                // }
                // else if (currentCell == GAMEOVER_2) {
                    // printf("OV");
                // }
                // else if (currentCell == GAMEOVER_3) {
                    // printf("ER");
                // }
                else {
                    printf("  ");
                }
                disableColor();
            }
        }
        printf("\n");

    }

    if (enableGhostBlock == 1) {
        eraseBlock(buffer, ghostBlock);
    }
    eraseBlock(buffer, block);

    clearScreen(0);
}

void drawGameBorder(struct Buffer *buffer)
{
    int x, y;
    for (x = 0; x < BUFFER_WIDTH; x+= BUFFER_WIDTH-1) {
        for (y = 0; y < ROW_FLOOR; y++) {
            setCell(buffer, x, y, FILL_WALL);
        }
    }
    for (x = 0; x < BUFFER_WIDTH; x++) {
        setCell(buffer, x, ROW_FLOOR, FILL_FLOOR);
    }
}

void drawGameOver(struct Buffer *buffer)
{
	buffer->dirty = 0;
    fillBuffer(buffer, 0);
    drawGameBorder(buffer);

    int i, j, x, y;
	for (j = 4, i = 0; i < ROW_FLOOR; i++) {
		setCell(buffer, j, i, GAMEOVER_0);
		setCell(buffer, j+1, i, GAMEOVER_1);
		setCell(buffer, j+2, i, GAMEOVER_2);
		setCell(buffer, j+3, i, GAMEOVER_3);
		for (y = 0; y < ROW_FLOOR; y++) {
			for (x = 0; x < BUFFER_WIDTH; x++) {
				uint8_t currentCell = buffer->content[y][x];
				if (currentCell == FILL_WALL) {
					printf("||");
				}
				else if (currentCell == FILL_FLOOR) {
					printf("::");
				}
				else if (currentCell == GAMEOVER_0) {
					printf("GA");
				}
				else if (currentCell == GAMEOVER_1) {
					printf("ME");
				}
				else if (currentCell == GAMEOVER_2) {
					printf("OV");
				}
				else if (currentCell == GAMEOVER_3) {
					printf("ER");
				}
				else {
					printf("  ");
				}
				disableColor();
			}
			printf("\n");
		}
		setCell(buffer, j, i, 0);
		setCell(buffer, j+1, i, 0);
		setCell(buffer, j+2, i, 0);
		setCell(buffer, j+3, i, 0);
		clearScreen(0);
		msleep(50);
	}
	msleep(800);
}


void drawBlock(struct Buffer *buffer, struct Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        setCell(
            buffer,
            block->x + getCoordX(block, MAIN, i),
            block->y + getCoordY(block, MAIN, i),
            block->fillType
        );
    }
}

void eraseBlock(struct Buffer *buffer, struct Block *block)
{
    int i;
    for (i = 0; i < 4; i++) {
        setCell(
            buffer,
            block->x + getCoordX(block, MAIN, i),
            block->y + getCoordY(block, MAIN, i),
            0
        );
    }
}

void drawPauseMessage(struct Buffer *buffer)
{
    buffer->content[ROW_LINE_COUNTER][0] = 0;
    buffer->content[ROW_SCORE][0] = 0;
    buffer->content[ROW_LEVEL][0] = 0;
    buffer->dirty = 1;
}

void erasePauseMessage(struct Buffer *buffer)
{
    buffer->content[ROW_LINE_COUNTER][0] = 1;
    buffer->content[ROW_SCORE][0] = 1;
    buffer->content[ROW_LEVEL][0] = 1;
    buffer->dirty = 1;
}
