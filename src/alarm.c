#include "include/alarm.h"
#include "utils/include/program.h"

#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>

pid_t alarm_pid = 0;

pid_t fork_to(void (*func)()) {
    pid_t p;
    p = fork();
    if(p<0) {
        error("Couldn't fork to start external process");
    }
    else if ( p == 0) {
        func();
        exit(0);
    }

    return p;
}

void start_ffplay() {
    // This function completely hands over the process. Don't call this on the main process, use fork_to(start_ffplay) instead!

    // > /dev/null 2>&1
    int dev_null = open("/dev/null", O_WRONLY);
    if (dev_null == -1) {
        error("Failed to open /dev/null");
        exit(1);
    }
    dup2(dev_null, STDOUT_FILENO);
    dup2(dev_null, STDERR_FILENO);
    close(dev_null);

    char* args[] = {"ffplay", "alarm.ogg", "-autoexit", "-nodisp", "-hide_banner", NULL};
    execvp("ffplay", args);
    error("Failed to start process");
}

void start_alarm() {
    alarm_pid = fork_to(start_ffplay);
}
void stop_alarm() {
    if (alarm_pid) {
        kill(alarm_pid, SIGTERM);
    }
}
