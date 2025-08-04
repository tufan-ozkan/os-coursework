#include "game_structs.h"
#include "print_output.h"
#include "grid_operations.h"
#include "utils.h"
#include "server.h"

int main() {
    int grid_width, grid_height, streak_size, player_count;
    Player *players;
    Grid    grid;

    read_configuration(&grid_width, &grid_height, &streak_size, &player_count, &players);
    init_grid(&grid, grid_width, grid_height);
    run_server(&grid, players, player_count, streak_size);

    free_grid(&grid);
    free_players(players, player_count);
    return 0;
}