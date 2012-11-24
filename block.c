#include "block.h"

struct Block *Block_create(void)
{
    struct Block *block = malloc(sizeof(struct Block));
    int i;
    for (i = 0; i < 4; i++) {
        block->coords[i].x                = 0;
        block->coords[i].y                = 0;
        block->leftCollisionCoords[i].x   = 0;
        block->leftCollisionCoords[i].y   = 0;
        block->rightCollisionCoords[i].x  = 0;
        block->rightCollisionCoords[i].y  = 0;
        block->bottomCollisionCoords[i].x = 0;
        block->bottomCollisionCoords[i].y = 0;
    }
    return block;
}

void Block_destroy(struct Block *block)
{
    free(block);
}

void rotateBlock(struct Block *block)
{
    if (block->type == BLOCK_O) {
        return;
    }

    if (block->type == BLOCK_I
        || block->type == BLOCK_S
        || block->type == BLOCK_Z
    ) {
        if (block->rotate == 1) {
            block->rotate = 0;
            setBlockType(block, block->type, block->rotate);
            return;
        }
    }
    else if (block->rotate == 3) {
        block->rotate = 0;
        setBlockType(block, block->type, block->rotate);
        return;
    }
    block->rotate++;
    setBlockType(block, block->type, block->rotate);
}

void setBlockRotate(struct Block *block, uint8_t rotate)
{
    if (block->type == BLOCK_O) {
        return;
    }

    if (block->type == BLOCK_I
        || block->type == BLOCK_S
        || block->type == BLOCK_Z
    ) {
        if (rotate > 1) {
            block->rotate = 0;
            setBlockType(block, block->type, block->rotate);
            return;
        }
    }
    else if (rotate > 3) {
        block->rotate = 0;
        setBlockType(block, block->type, block->rotate);
        return;
    }
    block->rotate = rotate;
    setBlockType(block, block->type, block->rotate);
}

void setBlockType(struct Block *block, uint8_t type, uint8_t rotate)
{
    clearCollisionCoords(block);
    block->type = type;
    block->rotate = rotate;
    switch (type) {
        case BLOCK_I:
            block->fillType = FILL_1;
            break;
        case BLOCK_J:
            block->fillType = FILL_2;
            break;
        case BLOCK_L:
            block->fillType = FILL_3;
            break;
        case BLOCK_O:
            block->fillType = FILL_4;
            break;
        case BLOCK_S:
            block->fillType = FILL_5;
            break;
        case BLOCK_T:
            block->fillType = FILL_6;
            break;
        case BLOCK_Z:
            block->fillType = FILL_7;
            break;
        default:
            break;
    }
    if (block->type == BLOCK_I && block->rotate == 0) {
        block->coords[0].x = -1;
        block->coords[0].y = 0;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = 1;
        block->coords[2].y = 0;
        block->coords[3].x = 2;
        block->coords[3].y = 0;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 0;

        block->rightCollisionCoords[0].x = 3;
        block->rightCollisionCoords[0].y = 0;

        block->bottomCollisionCoords[0].x = -1;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = 0;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 1;
        block->bottomCollisionCoords[3].x = 2;
        block->bottomCollisionCoords[3].y = 1;
    }
    else if (block->type == BLOCK_I && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = -1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = 1;
        block->coords[3].x = 0;
        block->coords[3].y = 2;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = -1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = 1;
        block->leftCollisionCoords[3].x = -1;
        block->leftCollisionCoords[3].y = 2;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = -1;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = 1;
        block->rightCollisionCoords[3].x = 1;
        block->rightCollisionCoords[3].y = 2;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 3;
    }
    else if (block->type == BLOCK_J && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = 1;
        block->coords[1].y = 0;
        block->coords[2].x = -1;
        block->coords[2].y = 0;
        block->coords[3].x = -1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = -1;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 0;
        block->rightCollisionCoords[1].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = -1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_J && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = -1;
        block->coords[3].x = 1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 2;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 0;
    }
    else if (block->type == BLOCK_J && block->rotate == 2) {
        block->coords[0].x = 1;
        block->coords[0].y = 1;
        block->coords[1].x = 1;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = 0;
        block->coords[3].x = -1;
        block->coords[3].y = 0;

        block->leftCollisionCoords[0].x = 0;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = 0;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 0;

        block->bottomCollisionCoords[0].x = 1;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = 0;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = -1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_J && block->rotate == 3) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = -1;
        block->coords[1].y = 1;
        block->coords[2].x = 0;
        block->coords[2].y = 0;
        block->coords[3].x = 0;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 2;
    }
    else if (block->type == BLOCK_L && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = -1;
        block->coords[1].y = 0;
        block->coords[2].x = 1;
        block->coords[2].y = 0;
        block->coords[3].x = 1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = 0;
        block->leftCollisionCoords[1].y = -1;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_L && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = 1;
        block->coords[1].y = 1;
        block->coords[2].x = 0;
        block->coords[2].y = 1;
        block->coords[3].x = 0;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 1;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 1;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 1;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = 0;
        block->bottomCollisionCoords[1].y = 2;
    }
    else if (block->type == BLOCK_L && block->rotate == 2) {
        block->coords[0].x = -1;
        block->coords[0].y = 1;
        block->coords[1].x = -1;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = 0;
        block->coords[3].x = 1;
        block->coords[3].y = 0;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = 0;

        block->rightCollisionCoords[0].x = 0;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 0;

        block->bottomCollisionCoords[0].x = -1;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = 0;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_L && block->rotate == 3) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = -1;
        block->coords[3].x = -1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -2;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 0;
    }
    else if (block->type == BLOCK_O && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = -1;
        block->coords[1].x = 1;
        block->coords[1].y = -1;
        block->coords[2].x = 0;
        block->coords[2].y = 0;
        block->coords[3].x = 1;
        block->coords[3].y = 0;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = -1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = -1;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 0;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 0;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 0;
        block->bottomCollisionCoords[2].x = 0;
        block->bottomCollisionCoords[2].y = 1;
        block->bottomCollisionCoords[3].x = 1;
        block->bottomCollisionCoords[3].y = 1;
    }
    else if (block->type == BLOCK_S && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = -1;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = -1;
        block->coords[3].x = 1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 0;
    }
    else if (block->type == BLOCK_S && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = 0;
        block->coords[1].y = -1;
        block->coords[2].x = -1;
        block->coords[2].y = -1;
        block->coords[3].x = -1;
        block->coords[3].y = -2;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = -1;
        block->leftCollisionCoords[2].x = -2;
        block->leftCollisionCoords[2].y = -2;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = -1;
        block->rightCollisionCoords[2].x = 0;
        block->rightCollisionCoords[2].y = -2;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 0;
    }
    else if (block->type == BLOCK_T && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = -1;
        block->coords[1].y = 0;
        block->coords[2].x = 1;
        block->coords[2].y = 0;
        block->coords[3].x = 0;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -2;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = -1;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_T && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = 1;
        block->coords[2].y = 0;
        block->coords[3].x = 0;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 1;
    }
    else if (block->type == BLOCK_T && block->rotate == 2) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = -1;
        block->coords[2].y = 0;
        block->coords[3].x = 1;
        block->coords[3].y = 0;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = 0;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = 0;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = 1;
        block->bottomCollisionCoords[2].y = 1;
    }
    else if (block->type == BLOCK_T && block->rotate == 3) {
        block->coords[0].x = 0;
        block->coords[0].y = 1;
        block->coords[1].x = 0;
        block->coords[1].y = 0;
        block->coords[2].x = -1;
        block->coords[2].y = 0;
        block->coords[3].x = 0;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 1;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = 0;
        block->leftCollisionCoords[2].x = -1;
        block->leftCollisionCoords[2].y = -1;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 1;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = 0;
        block->rightCollisionCoords[2].x = 1;
        block->rightCollisionCoords[2].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 2;
        block->bottomCollisionCoords[1].x = -1;
        block->bottomCollisionCoords[1].y = 1;
    }
    else if (block->type == BLOCK_Z && block->rotate == 0) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = 1;
        block->coords[1].y = 0;
        block->coords[2].x = 0;
        block->coords[2].y = -1;
        block->coords[3].x = -1;
        block->coords[3].y = -1;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -2;
        block->leftCollisionCoords[1].y = -1;

        block->rightCollisionCoords[0].x = 2;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 1;
        block->rightCollisionCoords[1].y = -1;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 1;
        block->bottomCollisionCoords[2].x = -1;
        block->bottomCollisionCoords[2].y = 0;
    }
    else if (block->type == BLOCK_Z && block->rotate == 1) {
        block->coords[0].x = 0;
        block->coords[0].y = 0;
        block->coords[1].x = 0;
        block->coords[1].y = -1;
        block->coords[2].x = 1;
        block->coords[2].y = -1;
        block->coords[3].x = 1;
        block->coords[3].y = -2;

        block->leftCollisionCoords[0].x = -1;
        block->leftCollisionCoords[0].y = 0;
        block->leftCollisionCoords[1].x = -1;
        block->leftCollisionCoords[1].y = -1;
        block->leftCollisionCoords[2].x = 0;
        block->leftCollisionCoords[2].y = -2;

        block->rightCollisionCoords[0].x = 1;
        block->rightCollisionCoords[0].y = 0;
        block->rightCollisionCoords[1].x = 2;
        block->rightCollisionCoords[1].y = -1;
        block->rightCollisionCoords[2].x = 2;
        block->rightCollisionCoords[2].y = -2;

        block->bottomCollisionCoords[0].x = 0;
        block->bottomCollisionCoords[0].y = 1;
        block->bottomCollisionCoords[1].x = 1;
        block->bottomCollisionCoords[1].y = 0;
    }
}

void clearCollisionCoords(struct Block *block)
{
    int i;
    for (i = 0; i < 4; i++){
        block->coords[i].x                = IGNORE;
        block->coords[i].y                = IGNORE;
        block->leftCollisionCoords[i].x   = IGNORE;
        block->leftCollisionCoords[i].y   = IGNORE;
        block->rightCollisionCoords[i].x  = IGNORE;
        block->rightCollisionCoords[i].y  = IGNORE;
        block->bottomCollisionCoords[i].x = IGNORE;
        block->bottomCollisionCoords[i].y = IGNORE;
    }
}


int getCoordX(struct Block *block, int coordinateType, int partNumber)
{
    if (coordinateType == MAIN) {
        return block->coords[partNumber].x;
    }
    else if (coordinateType == LEFT_COLLISION) {
        return block->leftCollisionCoords[partNumber].x;
    }
    else if (coordinateType == RIGHT_COLLISION) {
        return block->rightCollisionCoords[partNumber].x;
    }
    else if (coordinateType == BOTTOM_COLLISION) {
        return block->bottomCollisionCoords[partNumber].x;
    }
    return 0;
}

int getCoordY(struct Block *block, int coordinateType, int partNumber)
{
    if (coordinateType == MAIN) {
        return block->coords[partNumber].y;
    }
    else if (coordinateType == LEFT_COLLISION) {
        return block->leftCollisionCoords[partNumber].y;
    }
    else if (coordinateType == RIGHT_COLLISION) {
        return block->rightCollisionCoords[partNumber].y;
    }
    else if (coordinateType == BOTTOM_COLLISION) {
        return block->bottomCollisionCoords[partNumber].y;
    }
    return 0;
}
