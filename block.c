#include "block.h"

#include <stdlib.h>
#include <stdio.h>


Block *Block_create(void)
{
    Block *block = malloc(sizeof(Block));
    int i;
    for (i = 0; i < 4; i++) {
        block->coords[i].x                  = 0;
        block->coords[i].y                  = 0;
        block->left_collision_coords[i].x   = 0;
        block->left_collision_coords[i].y   = 0;
        block->right_collision_coords[i].x  = 0;
        block->right_collision_coords[i].y  = 0;
        block->bottom_collision_coords[i].x = 0;
        block->bottom_collision_coords[i].y = 0;
    }
    return block;
}

void Block_destroy(Block *block)
{
    free(block);
}

void Block_rotate(Block *block)
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
            Block_set_type(block, block->type, block->rotate);
            return;
        }
    }
    else if (block->rotate == 3) {
        block->rotate = 0;
        Block_set_type(block, block->type, block->rotate);
        return;
    }
    block->rotate++;
    Block_set_type(block, block->type, block->rotate);
}

void Block_set_rotate(Block *block, uint8_t rotate)
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
            Block_set_type(block, block->type, block->rotate);
            return;
        }
    }
    else if (rotate > 3) {
        block->rotate = 0;
        Block_set_type(block, block->type, block->rotate);
        return;
    }
    block->rotate = rotate;
    Block_set_type(block, block->type, block->rotate);
}

void Block_set_type(Block *block, BLOCK_TYPE type, uint8_t rotate)
{
    Block_clear_collision_coords(block);
    block->type = type;
    block->rotate = rotate;
    switch (type) {
        case BLOCK_I:
            block->fill_type = FILL_1;
            break;
        case BLOCK_J:
            block->fill_type = FILL_2;
            break;
        case BLOCK_L:
            block->fill_type = FILL_3;
            break;
        case BLOCK_O:
            block->fill_type = FILL_4;
            break;
        case BLOCK_S:
            block->fill_type = FILL_5;
            break;
        case BLOCK_T:
            block->fill_type = FILL_6;
            break;
        case BLOCK_Z:
            block->fill_type = FILL_7;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 0;

        block->right_collision_coords[0].x = 3;
        block->right_collision_coords[0].y = 0;

        block->bottom_collision_coords[0].x = -1;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = 0;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 1;
        block->bottom_collision_coords[3].x = 2;
        block->bottom_collision_coords[3].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = -1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = 1;
        block->left_collision_coords[3].x = -1;
        block->left_collision_coords[3].y = 2;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = -1;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = 1;
        block->right_collision_coords[3].x = 1;
        block->right_collision_coords[3].y = 2;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 3;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = -1;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 0;
        block->right_collision_coords[1].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = -1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 2;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 0;
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

        block->left_collision_coords[0].x = 0;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = 0;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 0;

        block->bottom_collision_coords[0].x = 1;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = 0;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = -1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 2;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = 0;
        block->left_collision_coords[1].y = -1;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 1;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 1;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 1;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = 0;
        block->bottom_collision_coords[1].y = 2;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = 0;

        block->right_collision_coords[0].x = 0;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 0;

        block->bottom_collision_coords[0].x = -1;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = 0;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -2;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 0;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = -1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = -1;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 0;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 0;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 0;
        block->bottom_collision_coords[2].x = 0;
        block->bottom_collision_coords[2].y = 1;
        block->bottom_collision_coords[3].x = 1;
        block->bottom_collision_coords[3].y = 1;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 0;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = -1;
        block->left_collision_coords[2].x = -2;
        block->left_collision_coords[2].y = -2;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = -1;
        block->right_collision_coords[2].x = 0;
        block->right_collision_coords[2].y = -2;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 0;
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

        block->left_collision_coords[0].x = -2;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = -1;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = 0;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = 0;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = 1;
        block->bottom_collision_coords[2].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 1;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = 0;
        block->left_collision_coords[2].x = -1;
        block->left_collision_coords[2].y = -1;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 1;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = 0;
        block->right_collision_coords[2].x = 1;
        block->right_collision_coords[2].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 2;
        block->bottom_collision_coords[1].x = -1;
        block->bottom_collision_coords[1].y = 1;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -2;
        block->left_collision_coords[1].y = -1;

        block->right_collision_coords[0].x = 2;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 1;
        block->right_collision_coords[1].y = -1;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 1;
        block->bottom_collision_coords[2].x = -1;
        block->bottom_collision_coords[2].y = 0;
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

        block->left_collision_coords[0].x = -1;
        block->left_collision_coords[0].y = 0;
        block->left_collision_coords[1].x = -1;
        block->left_collision_coords[1].y = -1;
        block->left_collision_coords[2].x = 0;
        block->left_collision_coords[2].y = -2;

        block->right_collision_coords[0].x = 1;
        block->right_collision_coords[0].y = 0;
        block->right_collision_coords[1].x = 2;
        block->right_collision_coords[1].y = -1;
        block->right_collision_coords[2].x = 2;
        block->right_collision_coords[2].y = -2;

        block->bottom_collision_coords[0].x = 0;
        block->bottom_collision_coords[0].y = 1;
        block->bottom_collision_coords[1].x = 1;
        block->bottom_collision_coords[1].y = 0;
    }
}

void Block_clear_collision_coords(Block *block)
{
    int i;
    for (i = 0; i < 4; i++){
        block->coords[i].x                  = COORDINATE_IGNORE;
        block->coords[i].y                  = COORDINATE_IGNORE;
        block->left_collision_coords[i].x   = COORDINATE_IGNORE;
        block->left_collision_coords[i].y   = COORDINATE_IGNORE;
        block->right_collision_coords[i].x  = COORDINATE_IGNORE;
        block->right_collision_coords[i].y  = COORDINATE_IGNORE;
        block->bottom_collision_coords[i].x = COORDINATE_IGNORE;
        block->bottom_collision_coords[i].y = COORDINATE_IGNORE;
    }
}

int Block_get_coord_x(Block *block, int coordinate_type, int part_number)
{
    if (coordinate_type == COORDINATE_MAIN) {
        return block->coords[part_number].x;
    }
    else if (coordinate_type == COORDINATE_LEFT_COLLISION) {
        return block->left_collision_coords[part_number].x;
    }
    else if (coordinate_type == COORDINATE_RIGHT_COLLISION) {
        return block->right_collision_coords[part_number].x;
    }
    else if (coordinate_type == COORDINATE_BOTTOM_COLLISION) {
        return block->bottom_collision_coords[part_number].x;
    }

    return 0;
}

int Block_get_coord_y(Block *block, int coordinate_type, int part_number)
{
    if (coordinate_type == COORDINATE_MAIN) {
        return block->coords[part_number].y;
    }
    else if (coordinate_type == COORDINATE_LEFT_COLLISION) {
        return block->left_collision_coords[part_number].y;
    }
    else if (coordinate_type == COORDINATE_RIGHT_COLLISION) {
        return block->right_collision_coords[part_number].y;
    }
    else if (coordinate_type == COORDINATE_BOTTOM_COLLISION) {
        return block->bottom_collision_coords[part_number].y;
    }

    return 0;
}
