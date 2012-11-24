#include "tetris.h"

struct Tetris *Tetris_create(void)
{
    srand(time(NULL));
    initScreen();
    struct Tetris *tetris = malloc(sizeof(struct Tetris));
    tetris->buffer = Buffer_create();
    fillBuffer(tetris->buffer, 0);
    tetris->currentBlock = Block_create();
    tetris->currentBlock->x = 5;
    tetris->currentBlock->y = 0;
    setBlockType(tetris->currentBlock, getRandomBlockType(), 0);

    tetris->gravityFrameCounter  = 0;
    tetris->movementFrameCounter = 0;
    tetris->movementFrameDelay   = 10;
    tetris->linesCompleted       = 0;
    tetris->linesUntilLevelUp    = 10;
    tetris->score                = 0;
    tetris->level                = 1;
    if (tetris->level < 16) {
        tetris->gravityFrameDelay = 16 - tetris->level;
    }
    else {
        tetris->gravityFrameDelay = 1;
    }

    tetris->gameState = RUNNING;
    tetris->colorMode = DEFAULT_COLOR_MODE;
    tetris->buffer->content[ROW_LINE_COUNTER][0] = 1;
    tetris->buffer->content[ROW_SCORE][0] = 1;
    tetris->buffer->content[ROW_LEVEL][0] = 1;
    drawGameBorder(tetris->buffer);
    tetris->buffer->dirty = 1;

    return tetris;
}

void Tetris_destroy(struct Tetris *tetris)
{
    Buffer_destroy(tetris->buffer);
    Block_destroy(tetris->currentBlock);
    free(tetris);
    cleanupScreen();
}

void initScreen(void)
{
    beginRawMode();
    setCursor(0);
    clearScreen(1);
}

void cleanupScreen(void)
{
    setCursor(1);
    endRawMode();
    clearScreen(1);
}

void gameLoop(struct Tetris *tetris)
{
    while (1) {
        getKeyInput(tetris);
        update(tetris);
        if (tetris->buffer->dirty) {
            drawFrame(tetris);
        }
        msleep(50);
    }
}

void getKeyInput(struct Tetris *tetris)
{
    tetris->currentKey = getKey(tetris->currentKeySequence);
}

void update(struct Tetris *tetris)
{
    if (collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
        tetris->movementFrameCounter++;
        if (tetris->movementFrameCounter == tetris->movementFrameDelay) {
            tetris->movementFrameCounter = 0;
            drawBlock(tetris->buffer, tetris->currentBlock);
            nextBlock(tetris);
            int completeLines = checkCompleteLines(tetris);
            checkGameOver(tetris);
            updateScore(tetris, completeLines);
            updateLevel(tetris);
            tetris->buffer->dirty = 1;
        }
    }

    if (tetris->currentKey == KEY_Q
        || tetris->currentKey == KEY_CONTROL_C
    ) {
        Tetris_destroy(tetris);
        exit(0);
    }
    else if (tetris->currentKey == KEY_P) {
        if (tetris->gameState == RUNNING) {
            tetris->gameState = PAUSED;
            drawPauseMessage(tetris->buffer);
        }
        else if (tetris->gameState == PAUSED) {
            tetris->gameState = RUNNING;
            erasePauseMessage(tetris->buffer);
        }
    }


    if (tetris->gameState != PAUSED) {
        if (!collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
            tetris->gravityFrameCounter++;
            if (tetris->gravityFrameCounter == tetris->gravityFrameDelay) {
                tetris->gravityFrameCounter = 0;
                tetris->currentBlock->y++;
                tetris->buffer->dirty = 1;
            }
        }
        if (tetris->currentKey == KEY_SPACE) {
            while (!collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)) {
                tetris->currentBlock->y++;
                tetris->score += 2;
            }
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_UP && rotateCollision(tetris->currentBlock, tetris->buffer) != 1) {
            rotateBlock(tetris->currentBlock);
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_LEFT
            && !collision(LEFT_COLLISION, tetris->currentBlock, tetris->buffer)) {
            tetris->currentBlock->x--;
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_RIGHT
                && !collision(RIGHT_COLLISION, tetris->currentBlock, tetris->buffer)
        ) {
            tetris->currentBlock->x++;
            tetris->buffer->dirty = 1;
        }
        else if (tetris->currentKey == KEY_DOWN
                && !collision(BOTTOM_COLLISION, tetris->currentBlock, tetris->buffer)
        ) {
            tetris->currentBlock->y++;
            tetris->score++;

            //if (this->enableGhostBlock) {
                //this->setGhostBlock();
            //}
            tetris->buffer->dirty = 1;
        }
    }
}

void drawFrame(struct Tetris *tetris)
{
    drawGame(
        tetris->buffer,
        tetris->currentBlock,
        tetris->colorMode,
        tetris->linesCompleted,
        tetris->score,
        tetris->level
    );
}

int checkCompleteLines(struct Tetris *tetris)
{
    int emptyCellCount = 0, lineCount = 0;
    int checkCount, x, y;
    for (checkCount = 0; checkCount < 4; checkCount++) {
        for (y = ROW_FLOOR-1; y > 0; y--) {
            for (x = 1; x < BUFFER_WIDTH-1; x++) {
                if (tetris->buffer->content[y][x] == EMPTY) {
                    emptyCellCount++;
                }
            }
            if (emptyCellCount == 0) {
                lineCount++;
                eraseLine(tetris->buffer, y);
                tetris->linesCompleted++;
            }
            emptyCellCount = 0;
        }
    }
    tetris->buffer->dirty = 1;
    return lineCount;
}


void eraseLine(struct Buffer *buffer, int lineNumber)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        setCell(buffer, i, lineNumber, EMPTY);
    }
    for (i = 1; i < ROW_FLOOR-1; i++) {
        if (lineNumber - i > 0) {
            dropLine(buffer, lineNumber - i);
        }
    }
}

void dropLine(struct Buffer *buffer, int lineNumber)
{
    int i;
    for (i = 1; i < BUFFER_WIDTH-1; i++) {
        setCell(buffer, i, lineNumber+1, buffer->content[lineNumber][i]);
        setCell(buffer, i, lineNumber, EMPTY);
    }
}

int collision(int collisionType, struct Block *block, struct Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (getCoordX(block, collisionType, i) == IGNORE
            || getCoordY(block, collisionType, i) == IGNORE
            || block->y + getCoordY(block, collisionType, i) < 0
        ) {
            continue;
        }
        if (getCell(
                buffer,
                block->x + getCoordX(block, collisionType, i),
                block->y + getCoordY(block, collisionType, i)
            ) != EMPTY
        ) {
            return 1;
        }
    }
    return 0;
}

int rotateCollision(struct Block *block, struct Buffer *buffer)
{
    if (block->type == BLOCK_O) {
        return 0;
    }
    int originalRotate = block->rotate;
    rotateBlock(block);
    if (collision(BOTTOM_COLLISION, block, buffer)
        || drawCollision(block, buffer)
    ) {
        setBlockRotate(block, originalRotate);
        return 1;
    }
    setBlockRotate(block, originalRotate);
    return 0;
}

int drawCollision(struct Block *block, struct Buffer *buffer)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (getCell(
                buffer,
                block->x + getCoordX(block, MAIN, i),
                block->y + getCoordY(block, MAIN, i)
            ) != EMPTY
            && getCell(
                buffer,
                block->x + getCoordX(block, MAIN, i),
                block->y + getCoordY(block, MAIN, i)
            ) != -1
        ) {
            return 1;
        }
    }
    return 0;
}

void nextBlock(struct Tetris *tetris)
{
    tetris->currentBlock->x = 5;
    tetris->currentBlock->y = 0;
    setBlockType(tetris->currentBlock, getRandomBlockType(), 0);

    //if (this->enableGhostBlock) {
        //this->setGhostBlock();
    //}
}

int getRandomBlockType(void)
{
    int min = 1, max = 7;
    return min + (rand() % (int)(max - min + 1));
}

void updateScore(struct Tetris *tetris, int lineCount)
{
    if (lineCount == 1) {
        tetris->score += 100 * tetris->level;
        tetris->linesUntilLevelUp -= 1;
    }
    else if (lineCount == 2) {
        tetris->score += 300 * tetris->level;
        tetris->linesUntilLevelUp -= 3;
    }
    else if (lineCount == 3) {
        tetris->score += 500 * tetris->level;
        tetris->linesUntilLevelUp -= 5;
    }
    else if (lineCount == 4) {
        tetris->score += 800 * tetris->level;
        tetris->linesUntilLevelUp -= 8;
    }
}

void updateLevel(struct Tetris *tetris)
{
    if (tetris->linesUntilLevelUp <= 0) {
        tetris->linesUntilLevelUp += 10;
        tetris->level++;
        if (tetris->gravityFrameDelay > 1) {
            tetris->gravityFrameDelay--;
        }
    }
}


void checkGameOver(struct Tetris *tetris)
{
    if (drawCollision(tetris->currentBlock, tetris->buffer)) {
        //drawGameOver();
        Tetris_destroy(tetris);
        exit(0);
    }
}
