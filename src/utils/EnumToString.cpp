#include "utils/EnumToString.h"

std::string renderModeToString(RenderMode mode) {
    switch (mode) {
    case RenderMode::SIMULATE: return "SIMULATE";
    case RenderMode::HARDWARE: return "HARDWARE";
    default: return "UNKNOWN";
    }
}

std::string colorModeToString(ColorMode mode) {
    switch (mode) {
    case ColorMode::STATIC: return "STATIC";
    case ColorMode::DYNAMIC: return "DYNAMIC";
    default: return "UNKNOWN";
    }
}
