#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "include/main.h"
#include "include/shared.h"
#include "include/alarm.h"
#include "include/player.h"
#include "sys/types.h"

#include <slib/program.h>
#include <slib/exceptions.h>
#include <slib/time.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s [command]\n", argv[0]);
        fprintf(stderr, "Try '%s help'\n", argv[0]);
        exit(1);
    }

    setup_slib_args(argv, argc);
    char* command;
    {
        char** pos_args = malloc(sizeof(char* ) * argc);
        u_long len = 1;
        get_pos_args_full(1, pos_args, &len);
        command = pos_args[0];

        free(pos_args);
    }
    atexit(stop_alarm);

    if (strcmp(command, "help") == 0)
        help();
    else if (strcmp(command, "test-alarm") == 0)
        interactive_alarm();
    else if (strcmp(command, "flat-alarm") == 0 || strcmp(command, "alarm") == 0)
        flat_alarm();
    else if (strcmp(command, "flat-timer") == 0 || strcmp(command, "timer") == 0)
        flat_timer();
    else if (strcmp(command, "term-clock") == 0 || strcmp(command, "clock") == 0)
        term_clock();
    else {
        fprintf(stderr, "Unknown command: %s\n", command);
        exit(1);
    }

    return 0;
}

void term_clock() {
    char* re = "\r\033[K";
    if (got_flag("-n")) {
        re = "\n";
    }

    int hours, minutes, seconds, new_secs;

    while (1) {
        get_time(&hours, &minutes, &seconds);

        printf("%02d:%02d:%02d", hours, minutes, seconds);
        fflush(stdout);

        new_secs = seconds;
        while (new_secs == seconds) {
            get_time(&hours, &minutes, &new_secs);
            delay(0.1);
        }
        printf("%s", re);
    }
}

void help() {
    printf(help_text, SLIB_args[0]);
}
