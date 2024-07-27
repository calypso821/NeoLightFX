#ifndef PERCISION_TIMER_H
#define PERCISION_TIMER_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

void setPercisionTimer();
void resetPercisionTimer();

#endif // PERCISION_TIMER_H
