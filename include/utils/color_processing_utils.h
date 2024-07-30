#ifndef COLOR_PROCESSING_UTILS_H
#define COLOR_PROCESSING_UTILS_H

#include <cstdint>

extern const float GAMMA;
extern const int GREYSCALE_THRESHOLD;
extern const float FACTOR_R;
extern const float FACTOR_G;
extern const float FACTOR_B;

uint8_t lerp(uint8_t a, uint8_t b, float t);
uint32_t applyTransitionCorrection(uint32_t oldColor, uint32_t newColor, int transitionSpeed);

uint32_t applyBrightnessCorrection(uint32_t color, int brightness);
uint32_t applyGreyscaleCorrection(uint32_t color);

#endif //COLOR_PROCESSING_UTILS_H