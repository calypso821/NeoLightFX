#include "utils/timer_utils.h"

void setPercisionTimer()
{
#if defined(_WIN32) || defined(_WIN64)
    // Set timer percision to 1ms (lowes possible on win)
    timeBeginPeriod(1);
#endif
}

void resetPercisionTimer()
{
#if defined(_WIN32) || defined(_WIN64)
    // Reset timer percision back to default (15.6ms)
    timeEndPeriod(1);
#endif
}