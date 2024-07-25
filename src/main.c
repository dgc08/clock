#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "include/main.h"
#include "include/shared.h"
#include "include/alarm.h"

#include "utils/include/program.h"
#include "utils/include/time.h"

char** args;
int args_len;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [command]\n", argv[0]);
        fprintf(stderr, "Try '%s help'\n", argv[0]);
        exit(1);
    }

    args = argv;
    args_len = argc;
    char* command = argv[1];

    atexit(stop_alarm);

    if (strcmp(command, "help") == 0)
        help();
    else if (strcmp(command, "test-alarm") == 0) {
        start_alarm();
        printf("Press enter to stop alarm> ");
        getchar();
    }
    else if (strcmp(command, "term-clock") == 0)
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
    printf("%s\n", help_text);
}
