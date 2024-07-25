#include "include/program.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int got_flag(const char* check) {
    for (int i = 0; i < args_len; i++) {
        if (strcmp(args[i], check) == 0) {
            return i;
        }
    }
    return false;
}

void error(const char* error_msg) {
    fprintf(stderr, "Error: %s\nErrno: ", error_msg);
    strerror(errno);
    exit(1);
}
