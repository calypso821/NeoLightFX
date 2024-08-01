#include "utils/color_processing_utils.h"

#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <algorithm>

// Logarithmic scale of brightness
const float GAMMA = 2.2f;

// Greyscale threshold (diffrence in colors)
const int GREYSCALE_THRESHOLD = 15;

// Coolor component scale factors
const float FACTOR_R = 1.0f;
const float FACTOR_G = 1.2f;
const float FACTOR_B = 0.8f;

uint8_t lerp(uint8_t a, uint8_t b, float t)
{
	return static_cast<uint8_t>(std::floor((b - a) * t + a));
}

uint32_t applyTransitionCorrection(uint32_t oldColor, uint32_t newColor, float tSpeed)
{
    float alpha = tSpeed / 100.0f;
    // TODO: Add dt (delta time)

    uint8_t oldRed = (oldColor >> 16) & 0xFF;
    uint8_t oldGreen = (oldColor >> 8) & 0xFF;
    uint8_t oldBlue = oldColor & 0xFF;

    uint8_t newRed = (newColor >> 16) & 0xFF;
    uint8_t newGreen = (newColor >> 8) & 0xFF;
    uint8_t newBlue = newColor & 0xFF;

    uint8_t blendedRed = lerp(oldRed, newRed, alpha);
    uint8_t blendedGreen = lerp(oldGreen, newGreen, alpha);
    uint8_t blendedBlue = lerp(oldBlue, newBlue, alpha);

    // Reassemble the 32-bit color value from the blended components
    return (blendedRed << 16) | (blendedGreen << 8) | blendedBlue;
}

uint32_t applyBrightnessCorrection(uint32_t color, int brightness)
{
    float factor = brightness / 100.f;
    // Gamma corrction
    factor = std::pow(factor, GAMMA);
    // Maybe I do linear? 

    int red = (color >> 16) & 0xFF;
    int green = (color >> 8) & 0xFF;
    int blue = color & 0xFF;

    // Adjust brightness
    red = static_cast<int>(std::round(std::min(255.0f, red * factor)));
    green = static_cast<int>(std::round(std::min(255.0f, green * factor)));
    blue = static_cast<int>(std::round(std::min(255.0f, blue * factor)));

    // Combine adjusted RGB components back into a single uint32_t
    return (red << 16) | (green << 8) | blue;
}


uint32_t applyGreyscaleCorrection(uint32_t color)
{
    int red = (color >> 16) & 0xFF;
    int green = (color >> 8) & 0xFF;
    int blue = color & 0xFF;

    // Check if color is greyscale specter (in range of 20)
    if (std::abs(red - green) < GREYSCALE_THRESHOLD &&
        std::abs(red - blue) < GREYSCALE_THRESHOLD &&
        std::abs(green - blue) < GREYSCALE_THRESHOLD)
    {
        // Values < 128 set to 0
        // Values > 128 map to [0...255]
        red = std::min(std::max(0, red - 127) << 1, 255);
        green = std::min(std::max(0, green - 127) << 1, 255);
        blue = std::min(std::max(0, blue - 127) << 1, 255);

        return (red << 16) | (green << 8) | blue;
    }

    return color;
}

std::string uint32ToString(uint32_t color) {
    // Extract RGB components
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    // Create a string stream
    std::stringstream ss;
    ss << static_cast<int>(r) << ", " << static_cast<int>(g) << ", " << static_cast<int>(b);

    return ss.str();
}