#ifndef SERVER_H
#define SERVER_H

#include "utils.h"
#include "print_output.h"
#include "grid_operations.h"

int     create_player_processes(Player *players, int player_count);
int     set_pset(Player *players, int player_count, fd_set *pset);

void    set_result_msg(sm *msg, int success, int filled_count);
void    send_and_print_result_msg(int fd, sm *server_msg, smp *smp_out, Grid *grid);
void    send_and_print_end_msg(Player *players, int player_count, char winner, int result, Grid *grid);
void    run_server(Grid *grid, Player *players, int player_count, int streak_size);

#endif 