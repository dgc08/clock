#include "include/alarm.h"
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

pid_t alarm_pid = 0;

char* alarm_audio = "alarm.ogg";

void start_ffplay() {
    disable_process_output(); // > /dev/null 2>&1

    char* args[] = {"ffplay", alarm_audio, "-autoexit", "-nodisp", "-hide_banner", NULL};
    execvp("ffplay", args);
    throw_error(""); // the output goes to /dev/null anyway so no msg
}

void start_alarm() {
    u_int pos = got_flag("-a"); // Check if other alarm should be used
    if (pos && pos < SLIB_args_len-1)
        alarm_audio = SLIB_args[pos+1];

    alarm_pid = fork_to(start_ffplay);
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

void flat_alarm() {
    char* re = "\r\033[K";
    if (got_flag("-n")) {
        re = "\n";
    }

    // Allocate SLIB_args_len (argc) for array 
    char** pos_args = malloc(sizeof(char*) * SLIB_args_len);
    size_t pos_args_len = SLIB_args_len;

    get_pos_args(2, pos_args, &pos_args_len);
    char* timer = pos_args[0];
    free(pos_args);

    if (pos_args_len > 0) {
        int timer_hours, timer_minutes, timer_seconds;
        int hours, minutes, seconds, new_secs;
        int diff_hours, diff_minutes, diff_seconds;

        if (sscanf(timer, "%d:%d:%d", &timer_hours, &timer_minutes, &timer_seconds) == 3) {
            printf("Set timer for %02d:%02d:%02d\nRemaining:\n", timer_hours, timer_minutes, timer_seconds);

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
                diff_hours += 60;
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
