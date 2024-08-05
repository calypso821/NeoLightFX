#ifndef COLOR_PROCESSING_UTILS_H
#define COLOR_PROCESSING_UTILS_H

#include <string>
#include <cstdint>

extern const float GAMMA;
extern const int GREYSCALE_THRESHOLD;
extern const float FACTOR_R;
extern const float FACTOR_G;
extern const float FACTOR_B;

uint32_t lerpColor(uint32_t color1, uint32_t color2, float t);
uint8_t lerp(uint8_t a, uint8_t b, float t);
uint32_t applyTransitionCorrection(uint32_t oldColor, uint32_t newColor, float tSpeed);

uint32_t applyBrightnessCorrection(uint32_t color, int brightness);
uint32_t applyGreyscaleCorrection(uint32_t color);

std::string uint32ToString(uint32_t color);

#endif //COLOR_PROCESSING_UTILS_H