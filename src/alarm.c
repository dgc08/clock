#include "include/player.h"

#include "lib/slib/types.h"

#include "include/alarm.h"
#include "sys/types.h"
#include "lib/slib/time.h"
#include "include/shared.h"
#include "lib/slib/program.h"

#include <stdio.h>
#include <stdlib.h>

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
