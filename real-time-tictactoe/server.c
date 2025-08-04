#include "server.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, fd)

int create_player_processes(Player *players, int player_count) {
    pid_t pid;

    for (int i = 0; i < player_count; i++) {
        if ((PIPE(players[i].pipefd)) < 0) {
            perror("pipe");
            return -1;
        }
        pid = fork();
        if (pid < 0) {
            perror("fork");
            return -1;
        }
        else if (pid == 0) { // child
            dup2(players[i].pipefd[0], 0);
            dup2(players[i].pipefd[0], 1);

            close(players[i].pipefd[1]);
            close(players[i].pipefd[0]);

            if (execv(players[i].args[0], players[i].args)) {
                perror("execv");
                return -1;
            }
        } else { // parent
            players[i].pid = pid;
            close(players[i].pipefd[0]);
        }
    }
    return 0;
}

int set_pset(Player *players, int player_count, fd_set *pset) {
    int maxfd = 0;
    FD_ZERO(pset);
    for (int i = 0; i < player_count; i++) {
        if (players[i].alive) {
            FD_SET(players[i].pipefd[1], pset);
            maxfd = (players[i].pipefd[1] > maxfd) ? players[i].pipefd[1] : maxfd;
        }
    }
    return maxfd;
}

void set_result_msg(sm *msg, int success, int filled_count) {
    msg->type = RESULT;
    msg->success = success;
    msg->filled_count = filled_count;
}

void send_and_print_result_msg(int fd, sm *server_msg, smp *server_msg_print, Grid *grid) {
    write(fd, server_msg, sizeof(sm));
    write(fd, (gu *) grid->marked_data, sizeof(gu) * grid->filled_count);

    print_output(NULL, server_msg_print, (gu *) grid->marked_data, grid->filled_count);
    //print_grid_state(grid);
}

void send_and_print_end_msg(Player *players, int player_count, char winner, int result, Grid *grid) {
    sm  end_msg = { .type = END };
    smp end_msg_print;

    for (int j = 0; j < player_count; j++) {
        write(players[j].pipefd[1], &end_msg, sizeof(sm));

        end_msg_print.process_id     = players[j].pid;
        end_msg_print.server_message = &end_msg; 
        
        print_output(NULL, &end_msg_print, NULL, 0);
    }
    
    //print_grid_state(grid);

    if (result == 1)
        printf("Winner: Player%c\n", winner);
    else
        printf("Draw\n");
}

void run_server(Grid *grid, Player *players, int player_count, int streak_size) {
    int active_players, maxfd, success, result;
    fd_set pset;
    struct timeval tv;
    cm client_msg;
    cmp client_msg_print;
    sm server_msg;
    smp server_msg_print;

    create_player_processes(players, player_count);

    active_players = player_count;
    while (active_players > 0) {
        maxfd = set_pset(players, player_count, &pset);

        tv.tv_sec  = 0; 
        tv.tv_usec = 1000;
        if (select(maxfd + 1, &pset, NULL, NULL, &tv) < 0) {
            break;
        }

        for (int i = 0; i < player_count; i++) {
            if (!players[i].alive) {
                continue;
            }
            if (!FD_ISSET(players[i].pipefd[1], &pset)) {
                continue;
            }
            if (read(players[i].pipefd[1], &client_msg, sizeof(cm)) == sizeof(cm)) {

                client_msg_print.process_id     = players[i].pid;
                client_msg_print.client_message = &client_msg;
                print_output(&client_msg_print, NULL, NULL, 0);

                server_msg_print.process_id     = players[i].pid;
                server_msg_print.server_message = &server_msg;

                if (client_msg.type == START) {
                    set_result_msg(&server_msg, 0, grid->filled_count);
                    send_and_print_result_msg(players[i].pipefd[1], &server_msg, &server_msg_print, grid);
                } 
                
                else if (client_msg.type == MARK) {
                    int success = mark_grid(grid, client_msg.position, players[i].character);

                    set_result_msg(&server_msg, success, grid->filled_count);
                    send_and_print_result_msg(players[i].pipefd[1], &server_msg, &server_msg_print, grid);

                    if (success) {
                        result = check_state(grid, client_msg.position, players[i].character, streak_size);

                        if (result == 1 || result == -1) {
                            send_and_print_end_msg(players, player_count, players[i].character, result, grid);
                            active_players = 0;
                            break;
                        }
                    }
                }

            } else {
                players[i].alive = 0;
                active_players--;

                close(players[i].pipefd[1]);
                waitpid(players[i].pid, NULL, 0);
            }
        }
    }
}