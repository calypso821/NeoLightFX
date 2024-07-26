#ifndef PLATFORM_DETECTION_H
#define PLATFORM_DETECTION_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

enum class Platform {
    Windows,
    Linux,
    Unknown
};

Platform getPlatform();

#endif // PLATFORM_DETECTION_H
