#ifndef LEDSTRIPCONFIG_H
#define LEDSTRIPCONFIG_H

#include <iostream>

struct LEDStripConfig {
    int ledNumWidth;
    int ledNumHeight;
    bool showBottom;

    LEDStripConfig(int ledNum_width, int ledNum_height, bool show_bottom)
        : ledNumWidth(ledNum_width),
        ledNumHeight(ledNum_height),
        showBottom(show_bottom) {}

    void toString() const {
        std::cout << "LED Strip: " << ledNumWidth << "x" << ledNumHeight
            << ", Show Bottom: " << (showBottom ? "Yes" : "No") << std::endl;
    }
};

#endif // LEDSTRIPCONFIG_H
