#ifndef TIMER_UTILS_H
#define TIMER_UTILS_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void setPercisionTimer();
void resetPercisionTimer();

#endif // TIMER_UTILS_H
