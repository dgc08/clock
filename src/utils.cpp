#include "include/utils.h"
#include "include/shared.h"

#include <time.h>

bool got_flag(std::string check) {
    for (size_t i = 0; i < args_len; i++) {
        if (args[i] == check) return true;
    }
    return false;
}

void get_time(int* hours, int* minutes, int* seconds) {
    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);

    *hours = now_tm->tm_hour;
    *minutes = now_tm->tm_min;
    *seconds = now_tm->tm_sec;
}
