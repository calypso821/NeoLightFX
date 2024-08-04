#ifndef LEDSTRIPCONFIG_H
#define LEDSTRIPCONFIG_H

#include <iostream>

struct LEDStripConfig {
    int width;
    int height;
    int count;
    bool bottom;

    // Constructor with width, height, and bottom
    LEDStripConfig(int ledNumWidth, int ledNumHeight, bool showBottom)
        : width(ledNumWidth),
        height(ledNumHeight),
        bottom(showBottom),
        count(ledNumWidth + ledNumHeight * 2 + ledNumWidth * showBottom) {}

    // Constructor with total LED count only
    LEDStripConfig(int ledCount)
        : width(0), // Default to 0, since it's not specified
        height(0), // Default to 0, since it's not specified
        count(ledCount),
        bottom(false) {} // Default to false, since it's not specified

    void toString() const {
        std::cout << "LED Strip: " << width << "x" << height
            << ", Show Bottom: " << (bottom ? "Yes" : "No") << std::endl
            << ", Total LED Count: " << count << std::endl;
    }
};

#endif // LEDSTRIPCONFIG_H
