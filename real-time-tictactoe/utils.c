#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void read_configuration(int *grid_width, int *grid_height, int *streak_size,
    int *player_count, Player **players) {

    scanf("%d %d %d %d", grid_width, grid_height, streak_size, player_count);
    *players = malloc(sizeof(Player) * (*player_count));

    for (int i = 0; i < *player_count; i++) {
        char char_p;
        int  total_arg_count;
        scanf(" %c %d", &char_p, &total_arg_count);

        (*players)[i].character = char_p;
        (*players)[i].arg_count = total_arg_count;
        (*players)[i].args      = malloc(sizeof(char *) * (total_arg_count + 2));

        for (int j = 0; j < total_arg_count + 1; j++) {
            (*players)[i].args[j] = malloc(256);
            scanf("%s", (*players)[i].args[j]);
        }

        (*players)[i].args[total_arg_count + 1] = NULL;
        (*players)[i].alive = 1;
    }
}

void free_players(Player *players, int player_count) {
    for (int i = 0; i < player_count; i++) {
        for (int j = 0; j < players[i].arg_count + 1; j++) {
            free(players[i].args[j]);
        }
        free(players[i].args);
        close(players[i].pipefd[1]);
    }
    free(players);
}

void print_grid_state(Grid *grid) {
    printf("\nCurrent Grid State:\n  ");

    for (int x = 0; x < grid->width; x++) {
        printf("%d ", x);
    }
    printf("\n");

    for (int y = 0; y < grid->height; y++) {
        printf("%d ", y);
        for (int x = 0; x < grid->width; x++) {
            char c = grid->rows[y][x];
            printf("%c ", (c == '\0') ? '.' : c);
        }
        printf("\n");
    }

    printf("\n");
}