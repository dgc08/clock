#include "include/time.h"

#include <time.h>
#include <unistd.h>

void delay(float msecs) {
    usleep(msecs*1000);
}

void get_time(int* hours, int* minutes, int* seconds) {
    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);

    *hours = now_tm->tm_hour;
    *minutes = now_tm->tm_min;
    *seconds = now_tm->tm_sec;
}
