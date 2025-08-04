#ifndef GRID_OPERATIONS_H
#define GRID_OPERATIONS_H

#include "game_structs.h"

typedef struct {
    int     width;
    int     height;
    int     filled_count;
    gd      *marked_data;  

    char    **rows;
    char    **cols;
    char    **diagonals;     
    char    **anti_diagonals;  
} Grid;

void init_grid(Grid *grid, int width, int height);
void free_grid(Grid *grid);
int  mark_grid(Grid *grid, coordinate pos, char character);
int  check_state(Grid *grid, coordinate pos, char character, int streak_size);

#endif