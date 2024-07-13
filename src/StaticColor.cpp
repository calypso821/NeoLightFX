#include "StaticColor.h"

StaticColor::StaticColor(int width, int height) {
    this->width = width;
    this->height = height;
}

void StaticColor::setColorByString(std::string color, ws2811_led_t* array) {
    if (color == "red") {
        setColor(0xFF0000, array);
    } else if (color == "green") {
        setColor(0x00FF00, array);
    } else if (color == "blue") {
        setColor(0x0000FF, array);
    } else if (color == "orange") {
        setColor(0xFFA500, array);
    }
}

void StaticColor::setColorByHex(uint32_t color, ws2811_led_t* array) {
    setColor(color, array);
}

void StaticColor::setColor(uint32_t color, ws2811_led_t* array) {
    for (int i = 0; i < LED_COUNT; i++) {
        array[i] = color;
    }
}
