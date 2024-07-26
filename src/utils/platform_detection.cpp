#include "utils/platform_detection.h"

Platform getPlatform() {
#if defined(_WIN32) || defined(_WIN64)
    return Platform::Windows;
#elif defined(__linux__)
    return Platform::Linux;
#else
    return Platform::Unknown;
#endif
}
