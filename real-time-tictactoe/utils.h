#ifndef UTILS_H
#define UTILS_H

#include "grid_operations.h"

typedef struct {
    char    character;
    int     arg_count;
    char  **args;
    int     pipefd[2];
    pid_t   pid;
    int     alive;
} Player;

void read_configuration(int *grid_width, int *grid_height, int *streak_size, int *player_count, Player **players);
void free_players(Player *players, int player_count);
void print_grid_state(Grid *grid);

#endif