#ifndef ENUMTOSTRING_H
#define ENUMTOSTRING_H

#include <string>
#include "RenderManager.h" // Include the file where your enums are defined

std::string renderModeToString(RenderMode mode);
std::string colorModeToString(ColorMode mode);

#endif // ENUMTOSTRING_H
