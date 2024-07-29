#include "include/player.h"

#include <slib/time.h>
#include <slib/program.h>
#include <slib/exceptions.h>
#include <slib/process.h>

#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

pid_t alarm_pid = 0;

const char* orig_player_args[32] = {"ffplay", "alarm.ogg", "-autoexit", "-nodisp", "-hide_banner", NULL};
char** player_args = (char** )orig_player_args;

void start_player() {
    disable_process_output(); // > /dev/null 2>&1

    execvp(player_args[0], player_args);
    throw_error(""); // the output goes to /dev/null anyway so no msg
}

void start_player_extra() {
    execvp(player_args[0], player_args);
    throw_error("Error starting extra process"); // the output goes to /dev/null anyway so no msg
}

void start_alarm() {
    char* arg = get_named_argument("-a="); // Check if other alarm should be used
    if (arg)
        player_args[1] = arg;

    arg = get_named_argument("-b="); // Check if other backend should be used
    if (arg) {
        player_args[0] = arg;
        player_args[2] = NULL;
    }

    if (got_flag("-d"))
        start_player();
    else
        alarm_pid = fork_to(start_player);
}
void stop_alarm() {
    if (alarm_pid) {
        kill(alarm_pid, SIGTERM);
    }
}

void interactive_alarm() {
    start_alarm();
    printf("Press enter to quit> ");
    getchar();
}


void check_multiple(char** pos_args, size_t len) {
    // This is kinda useless just open multiple processes
    // It was fun to implement tho
    if (len > 1) {
       char** new_args = malloc(sizeof(char*) * (SLIB_args_len+1));
        size_t i;

        for (i = 0; i < got_flag("--")-1; i++) {
            new_args[i] = SLIB_args[i];
        }

        /* new_args[i++] = "-d"; */
        new_args[i++] = "-n";
        new_args[i++] = "--";
        player_args = new_args;

        pid_t* pids = malloc(sizeof(pid_t) * len+1);

        for (size_t j = 0; j < len; j++) {
            player_args[i] = pos_args[j];
            player_args[i+1] = NULL;

            pid_t pid = fork_to(start_player_extra);
            pids[j] = pid;
            player_args[i-2] = "-c";
        }

        while (wait(NULL) > 0) {
            // wait(NULL) will return the PID of the child process that terminated
            // When 0 then there is no process left
            delay(1);
        }

        /* puts("Press enter to stop all> "); */
        /* getchar(); */
        /* for (size_t j = 0; j < len; j++) { */
        /*     printf("Kill %d at %zu len %zu\n", pids[j], j, len); */
        /*     kill(pids[j], SIGTERM); */
        /* } */

        free(new_args);
        free(pids);
        free(pos_args);
        exit(0);
    }
}
