#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>

#include "terminal.h"
#include "tetris.h"


static void set_color(Renderer *renderer, COLOR color)
{
    switch (color) {
        case COLOR_NONE:
            renderer->color = 0;
            break;

        case COLOR_WHITE:
            renderer->color = XTERM_256_WHITE;
            break;

        case COLOR_GRAY:
            renderer->color = XTERM_256_GRAY;
            break;

        case COLOR_RED:
            renderer->color = XTERM_256_RED;
            break;

        case COLOR_GREEN:
            renderer->color = XTERM_256_GREEN;
            break;

        case COLOR_YELLOW:
            renderer->color = XTERM_256_YELLOW;
            break;

        case COLOR_BLUE:
            renderer->color = XTERM_256_BLUE;
            break;

        case COLOR_PURPLE:
            renderer->color = XTERM_256_PURPLE;
            break;

        case COLOR_CYAN:
            renderer->color = XTERM_256_CYAN;
            break;

        case COLOR_ORANGE:
            renderer->color = XTERM_256_ORANGE;
            break;

        default:
            break;
    }
}

static void Renderer_draw_panel_value(Renderer *renderer,
                                      uint32_t value,
                                      unsigned int row,
                                      char **value_buffer)
{
    unsigned int value_width = (renderer->panel_value_width * 2) + 1;
    unsigned int padding_width = (renderer->panel_value_width * 2) - 1;

    char format[16];
    sprintf(format, "%s%u%s", " %-", padding_width, "u");

    char value_formatted[value_width];
    snprintf(value_formatted, renderer->panel_value_width*2, format, value);

    int i;
    for (i = 0; i < renderer->panel_value_width; i++) {
        sprintf(value_buffer[i],
                "%c%c",
                value_formatted[2*i], value_formatted[(2*i)+1]);
    }

    int x, y, value_index;

    value_index = 0;
    y = row;
    for (x = renderer->panel_label_width; x < renderer->buffer->width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);

        if (value_index < renderer->panel_value_width) {
            Buffer_set_pixel_value(renderer->buffer,
                                   x, y,
                                   value_buffer[value_index]);

            value_index++;
        }
    }
}

static void Renderer_erase_panel_value(Renderer *renderer, unsigned int row)
{
    int x;
    for (x = renderer->panel_label_width; x < renderer->buffer->width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, row, 0);
    }
}

Renderer *Renderer_create(unsigned int width,
                          unsigned int height,
                          unsigned char color_mode)
{
    Renderer *renderer = malloc(sizeof(Renderer));

    renderer->buffer = Buffer_create(width, height);

    renderer->color_mode = color_mode;

    renderer->color = XTERM_256_WHITE;

    renderer->left_wall          = 1;
    renderer->right_wall         = width - 2;

    renderer->row_floor          = height - 4;
    renderer->row_line_counter   = height - 3;
    renderer->row_score          = height - 2;
    renderer->row_level          = height - 1;

    renderer->fill_types[FILL_SOLID]      = "  ";
    renderer->fill_types[FILL_WALL]       = "||";
    renderer->fill_types[FILL_FLOOR]      = "::";
    renderer->fill_types[FILL_GAMEOVER_0] = "Ga";
    renderer->fill_types[FILL_GAMEOVER_1] = "me";
    renderer->fill_types[FILL_GAMEOVER_2] = "Ov";
    renderer->fill_types[FILL_GAMEOVER_3] = "er";

    renderer->panel_label_width = 3;
    renderer->panel_value_width = 9;

    int y;
    renderer->panel_buffer_line_counter = malloc(renderer->panel_value_width * sizeof(char *));
    for (y = 0; y < renderer->panel_value_width; y++) {
        renderer->panel_buffer_line_counter[y] = malloc(3 * sizeof(char));
    }

    renderer->panel_buffer_score = malloc(renderer->panel_value_width * sizeof(char *));
    for (y = 0; y < renderer->panel_value_width; y++) {
        renderer->panel_buffer_score[y] = malloc(3 * sizeof(char));
    }

    renderer->panel_buffer_level = malloc(renderer->panel_value_width * sizeof(char *));
    for (y = 0; y < renderer->panel_value_width; y++) {
        renderer->panel_buffer_level[y] = malloc(3 * sizeof(char));
    }

    return renderer;
}

void Renderer_destroy(Renderer *renderer)
{
    Buffer_destroy(renderer->buffer);

    int i;
    for(i = 0; i < renderer->panel_value_width; i++) {
        free(renderer->panel_buffer_line_counter[i]);
    }
    free(renderer->panel_buffer_line_counter);

    for(i = 0; i < renderer->panel_value_width; i++) {
        free(renderer->panel_buffer_score[i]);
    }
    free(renderer->panel_buffer_score);

    for(i = 0; i < renderer->panel_value_width; i++) {
        free(renderer->panel_buffer_level[i]);
    }
    free(renderer->panel_buffer_level);

    free(renderer);
}

void Renderer_present_buffer(Renderer *renderer)
{
    unsigned int x, y;
    for (y = 0; y < renderer->buffer->height; y++) {
        for (x = 0; x < renderer->buffer->width; x++) {
            Pixel pixel = renderer->buffer->pixel_data[y][x];

            if (pixel.enabled) {
                Terminal_set_color(pixel.foreground_color,
                                   pixel.background_color,
                                   pixel.bold);

                printf(pixel.value);

                Terminal_disable_color();
            }
            else {
                printf("  ");
            }
        }

        printf("\n");
    }

    Terminal_clear_screen(0);
}

void Renderer_draw_block(Renderer *renderer, Block *block)
{
    set_color(renderer, block->color);

    int i;
    for (i = 0; i < 4; i++) {
        Buffer_set_pixel_enabled(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            1);

        Buffer_set_pixel_foreground_color(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            renderer->color);

        Buffer_set_pixel_background_color(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            renderer->color);

        Buffer_set_pixel_value(
            renderer->buffer,
            block->x + Block_get_coord_x(block, COORDINATE_MAIN, i),
            block->y + Block_get_coord_y(block, COORDINATE_MAIN, i),
            renderer->fill_types[FILL_SOLID]);
    }
}

void Renderer_erase_block(Renderer *renderer, Block *block)
{
    unsigned int i;
    for (i = 0; i < 4; i++) {
        int x = block->x + Block_get_coord_x(block, COORDINATE_MAIN, i);
        int y = block->y + Block_get_coord_y(block, COORDINATE_MAIN, i);

        Buffer_set_pixel_enabled(renderer->buffer, x, y, 0);
    }
}

void Renderer_draw_pause_message(Renderer *renderer)
{
    const char *pause_message[10] = {
        "Pr", "es", "s ", "P ",

        "to", " C", "on", "ti", "nu", "e ",
    };

    int x, y, message_index;

    message_index = 0;

    y = renderer->row_line_counter;
    for (x = renderer->panel_label_width+1; x < renderer->panel_value_width-1; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer, x, y, pause_message[message_index]);

        message_index++;
    }

    y = renderer->row_score;
    for (x = renderer->panel_label_width; x < renderer->panel_value_width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer, x, y, pause_message[message_index]);

        message_index++;
    }

    renderer->buffer->dirty = 1;
}

void Renderer_erase_pause_message(Renderer *renderer)
{
    int x, y;

    y = renderer->row_line_counter;
    for (x = renderer->panel_label_width+1; x < renderer->buffer->width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer,
                               x, y,
                               renderer->fill_types[FILL_SOLID]);
    }

    y = renderer->row_score;
    for (x = renderer->panel_label_width; x < renderer->buffer->width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer,
                               x, y,
                               renderer->fill_types[FILL_SOLID]);
    }

    renderer->buffer->dirty = 1;
}

void Renderer_draw_panel_labels(Renderer *renderer)
{
    const char *panel_labels[9] = {
        "Li", "ne", "s:",

        "Sc", "or", "e:",

        "Le", "ve", "l:",
    };

    int x, y, label_index;

    label_index = 0;

    y = renderer->row_line_counter;
    for (x = 0; x < renderer->panel_label_width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer, x, y, panel_labels[label_index]);

        label_index++;
    }

    y = renderer->row_score;
    for (x = 0; x < renderer->panel_label_width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer, x, y, panel_labels[label_index]);

        label_index++;
    }

    y = renderer->row_level;
    for (x = 0; x < renderer->panel_label_width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);
        Buffer_set_pixel_value(renderer->buffer, x, y, panel_labels[label_index]);

        label_index++;
    }
}

void Renderer_erase_panel_labels(Renderer *renderer)
{
    int x;
    for (x = 0; x < renderer->panel_label_width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, renderer->row_line_counter, 0);
        Buffer_set_pixel_enabled(renderer->buffer, x, renderer->row_score, 0);
        Buffer_set_pixel_enabled(renderer->buffer, x, renderer->row_level, 0);
    }
}

void Renderer_draw_panel_line_counter(Renderer *renderer,
                                      uint32_t lines_completed)
{
    Renderer_draw_panel_value(renderer,
                              lines_completed,
                              renderer->row_line_counter,
                              renderer->panel_buffer_line_counter);
}

void Renderer_erase_panel_line_counter(Renderer *renderer)
{
    Renderer_erase_panel_value(renderer, renderer->row_line_counter);
}

void Renderer_draw_panel_score(Renderer *renderer,
                               uint32_t score)
{
    Renderer_draw_panel_value(renderer,
                              score,
                              renderer->row_score,
                              renderer->panel_buffer_score);
}

void Renderer_erase_panel_score(Renderer *renderer)
{
    Renderer_erase_panel_value(renderer, renderer->row_score);
}

void Renderer_draw_panel_level(Renderer *renderer,
                               uint32_t level)
{
    Renderer_draw_panel_value(renderer,
                              level,
                              renderer->row_level,
                              renderer->panel_buffer_level);
}

void Renderer_erase_panel_level(Renderer *renderer)
{
    Renderer_erase_panel_value(renderer, renderer->row_level);
}

void Renderer_draw_game_border(Renderer *renderer)
{
    renderer->color = XTERM_256_GRAY;

    int x, y;
    for (x = 0; x < renderer->buffer->width; x+= renderer->buffer->width-1) {
        for (y = 0; y < renderer->row_floor; y++) {
            Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);

            Buffer_set_pixel_foreground_color(renderer->buffer,
                                              x, y,
                                              renderer->color);

            Buffer_set_pixel_value(renderer->buffer,
                                   x, y,
                                   renderer->fill_types[FILL_WALL]);
        }
    }

    for (x = 0; x < renderer->buffer->width; x++) {
        Buffer_set_pixel_enabled(renderer->buffer, x, y, 1);

        Buffer_set_pixel_foreground_color(renderer->buffer,
                                          x, y,
                                          renderer->color);

        Buffer_set_pixel_value(renderer->buffer,
                               x, y,
                               renderer->fill_types[FILL_FLOOR]);
    }
}

void Renderer_draw_game_over(Renderer *renderer)
{

}
