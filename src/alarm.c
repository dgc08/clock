#include "include/alarm.h"
#include "sys/types.h"
#include "utils/include/time.h"
#include "include/shared.h"
#include "lib/slib/program.h"

#include "lib/slib/process.h"
#include "lib/slib/program.h"
#include "lib/slib/types.h"

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

pid_t alarm_pid = 0;

const char* orig_player_args[32] = {"ffplay", "alarm.ogg", "-autoexit", "-nodisp", "-hide_banner", NULL};
char** player_args = orig_player_args;

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
    u_int pos = got_flag("-a"); // Check if other alarm should be used
    if (pos && pos < SLIB_args_len-1)
        player_args[1] = SLIB_args[pos];

    pos = got_flag("-b"); // Check if other backend should be used
    if (pos && pos < SLIB_args_len-1) {
        player_args[0] = SLIB_args[pos];
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

void flat_alarm() {
    char* re = "\r\033[K";
    if (got_flag("-n")) {
        re = "\n";
    }
    if (got_flag("-c")) {
        re = " ";
    }

    // Allocate SLIB_args_len (argc) for array 
    char** pos_args = malloc(sizeof(char*) * SLIB_args_len);
    size_t pos_args_len = SLIB_args_len;

    get_pos_args(2, pos_args, &pos_args_len);
    check_multiple(pos_args, pos_args_len);

    char* timer = pos_args[0];
    free(pos_args);

    if (pos_args_len > 0) {
        int timer_hours, timer_minutes, timer_seconds;
        int hours, minutes, seconds, new_secs;
        int diff_hours, diff_minutes, diff_seconds;

        if (sscanf(timer, "%d:%d:%d", &timer_hours, &timer_minutes, &timer_seconds) == 3 || sscanf(timer, "%d.%d.%d", &timer_hours, &timer_minutes, &timer_seconds) == 3) {
            printf("Set alarm for %02d:%02d:%02d\nRemaining:\n", timer_hours, timer_minutes, timer_seconds);

            do {
            get_time(&hours, &minutes, &seconds);

            diff_hours = (timer_hours - hours) % 60;
            diff_minutes = (timer_minutes - minutes) % 60;
            diff_seconds = (timer_seconds - seconds) % 60;

            if (diff_seconds < 0) {
                diff_minutes--;
                diff_seconds += 60;
            }
            if (diff_minutes < 0) {
                diff_hours--;
                diff_minutes += 60;
            }
            if (diff_hours < 0) {
                diff_hours += 24;
            }

            printf("%s%02d:%02d:%02d", re, diff_hours, diff_minutes, diff_seconds);
            fflush(stdout);

            new_secs = seconds;
            while (new_secs == seconds) {
                get_time(&hours, &minutes, &new_secs);
                delay(0.1);
            }

            } while (diff_hours != 0 || diff_minutes != 0 || diff_seconds != 0 );

            printf("\n");
            interactive_alarm();


            return;
        }
    }
    throw_error("Invalid time format");
}


void flat_timer() {
    char* re = "\r\033[K";
    if (got_flag("-n")) {
        re = "\n";
    }
    if (got_flag("-c")) {
        re = " ";
    }

    // Allocate SLIB_args_len (argc) for array
    char** pos_args = malloc(sizeof(char*) * SLIB_args_len);
    size_t pos_args_len = SLIB_args_len;

    get_pos_args(2, pos_args, &pos_args_len);
    check_multiple(pos_args, pos_args_len);


    char* timer = pos_args[0];

    /* puts(""); */
    /* for (int i = 0; i < SLIB_args_len; i++) { */
    /*     printf("SLIB_args[%d] = %s\n", i, SLIB_args[i]); */
    /* } */
    free(pos_args);

    if (pos_args_len > 0) {
        int diff_hours, diff_minutes, diff_seconds;

        if (sscanf(timer, "%d:%d:%d", &diff_hours, &diff_minutes, &diff_seconds) == 3 || sscanf(timer, "%d.%d.%d", &diff_hours, &diff_minutes, &diff_seconds) == 3) {
            printf("Set alarm for %02d:%02d:%02d\nRemaining:\n", diff_hours, diff_minutes, diff_seconds);

            do {
            diff_seconds--;

            if (diff_seconds < 0) {
                diff_minutes--;
                diff_seconds += 60;
            }
            if (diff_minutes < 0) {
                diff_hours--;
                diff_minutes += 60;
            }
            if (diff_hours < 0) {
                diff_hours += 60;
            }

            printf("%s%02d:%02d:%02d", re, diff_hours, diff_minutes, diff_seconds);
            fflush(stdout);

            delay(1000);

            } while (diff_hours != 0 || diff_minutes != 0 || diff_seconds != 0 );

            printf("\n");
            interactive_alarm();

            return;
        }
    }
    throw_error("Invalid time format");
}
