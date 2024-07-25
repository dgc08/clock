#ifndef UTILS_H_
#define UTILS_H_

#include "shared.h"
#include <string.h>

void delay(unsigned int milliseconds);
bool got_flag(const char *check);
void get_time(int* hours, int* minutes, int* seconds);

#endif // UTILS_H_
