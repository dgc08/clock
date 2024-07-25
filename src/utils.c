#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "include/utils.h"
void delay(unsigned int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds*1000);
    #endif
}

void get_time(int* hours, int* minutes, int* seconds) {
    time_t now = time(NULL);
    struct tm* now_tm = localtime(&now);

    *hours = now_tm->tm_hour;
    *minutes = now_tm->tm_min;
    *seconds = now_tm->tm_sec;
}

bool got_flag(const char* check) {
    for (int i = 0; i < args_len; i++) {
        if (strcmp(args[i], check) == 0) {
            return true;
        }
    }
    return false;
}
