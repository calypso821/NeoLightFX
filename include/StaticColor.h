#ifndef STATICCOLOR_H
#define STATICCOLOR_H

#include <string>
#include <ws2811.h>

class StaticColor {
public:
    StaticColor(int width, int height);
    void setColorByString(std::string color, ws2811_led_t* array);
    void setColorByHex(uint32_t color, ws2811_led_t* array);

private:
    int width;
    int height;
    void setColor(uint32_t color, ws2811_led_t* array);
};

#endif // STATICCOLOR_H
