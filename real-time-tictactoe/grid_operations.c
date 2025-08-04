#include <stdlib.h>
#include <string.h>
#include "grid_operations.h"

void init_grid(Grid *grid, int width, int height) {
    grid->width = width;
    grid->height = height;
    grid->filled_count = 0;

    // rows
    grid->rows = malloc(sizeof(char *) * height);
    for (int y = 0; y < height; y++) {
        grid->rows[y] = calloc(width, sizeof(char));
    }

    // columns
    grid->cols = malloc(sizeof(char *) * width);
    for (int x = 0; x < width; x++) {
        grid->cols[x] = calloc(height, sizeof(char));
    }

    int diagonal_count = width + height - 1;
    int largest_dim = (width > height) ? width : height;
    // diagonals
    grid->diagonals = malloc(sizeof(char *) * diagonal_count);
    for (int i = 0; i < diagonal_count; i++) {
        grid->diagonals[i] = calloc(largest_dim, sizeof(char));
    }

    // anti-diagonals
    grid->anti_diagonals = malloc(sizeof(char *) * diagonal_count);
    for (int i = 0; i < diagonal_count; i++) {
        grid->anti_diagonals[i] = calloc(largest_dim, sizeof(char));
    }

    // all grid
    grid->marked_data = malloc(sizeof(gd) * width * height);
}

int mark_grid(Grid *grid, coordinate pos, char character) {
    if (pos.x < 0 || pos.x >= grid->width 
        || pos.y < 0 || pos.y >= grid->height) {
            return 0; // out of range
    } 
    else if (grid->rows[pos.y][pos.x] != '\0') {
        return 0; // already marked
    } 
    else {
        grid->rows[pos.y][pos.x] = character;
        grid->cols[pos.x][pos.y] = character;

        int diag_key    = pos.x - pos.y + (grid->height - 1);
        int diag_index  = (pos.x < pos.y) ? pos.x : pos.y;
        grid->diagonals[diag_key][diag_index] = character;

        int anti_diag_key   = pos.x + pos.y;
        int anti_diag_index = (pos.x < grid->width - pos.y) ? pos.x : grid->width - pos.y - 1;
        grid->anti_diagonals[anti_diag_key][anti_diag_index] = character;

        grid->marked_data[grid->filled_count].position.x = pos.x;
        grid->marked_data[grid->filled_count].position.y = pos.y;
        grid->marked_data[grid->filled_count].character = character;

        grid->filled_count++;
        return 1;
    }
}

int is_streak(char *line, int len, char character, int streak_size) {
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (line[i] == character) {
            count++;
            if (count >= streak_size) return 1;
        } else {
            count = 0;
        }
    }
    return 0;
}

int check_state(Grid *grid, coordinate pos, char character, int streak_size) {
    int largest_dim = (grid->width > grid->height) ? grid->width : grid->height; 
    int diag_key = pos.x - pos.y + (grid->height - 1);
    int anti_diag_key = pos.x + pos.y;

    if (is_streak(grid->rows[pos.y], grid->width, character, streak_size)) {
        return 1; // horizontal streak
    }
    if (is_streak(grid->cols[pos.x], grid->height, character, streak_size)) {
        return 1; // vertical streak
    }
    if (is_streak(grid->diagonals[diag_key], largest_dim, character, streak_size)) {
        return 1; // diagonal streak
    }
    if (is_streak(grid->anti_diagonals[anti_diag_key], largest_dim, character, streak_size)) {
        return 1; // anti-diagonal streak
    }
    if (grid->filled_count == (grid->width * grid->height)) {
        return -1; // draw
    }
    return 0; // continue
}

void free_grid(Grid *grid) {
    for (int i = 0; i < grid->height; i++) {
        free(grid->rows[i]);
    }

    for (int i = 0; i < grid->width; i++) {
        free(grid->cols[i]);
    }

    int diagonal_count = grid->width + grid->height - 1;
    for (int i = 0; i < diagonal_count; i++) {
        free(grid->diagonals[i]);
        free(grid->anti_diagonals[i]);
    }

    free(grid->rows);
    free(grid->cols);
    free(grid->diagonals);
    free(grid->anti_diagonals);
    free(grid->marked_data);
}
