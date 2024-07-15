#ifndef LEDCOLORCONTROLLER_H
#define LEDCOLORCONTROLLER_H

// #include "FrameProcessor.h"
// #include "StaticColor.h"
// #include <ws2811.h>
// #include <opencv2/opencv.hpp>
#include <cstdint>

enum class Color {
    Red = 0xFF0000,
    Green = 0x00FF00,
    Blue = 0x0000FF,
    White = 0xFFFFFF,
    Black = 0x000000
};



class LEDColorController 
{
public:
    LEDColorController(int ledNum_width, int ledNum_height, bool showBotttom);
    ~LEDColorController();

    uint8_t* getColorArray();
    int getColorArraySize();

    void setColorByName(Color color);
    void setColorByHexCode(uint32_t color);

private:
    int m_ledNum_width;
    int m_ledNum_height;

    int m_brightness;
    bool m_showBottom;

    

    int m_colorArraySize;
    uint8_t* m_pColorArray;

    void initColorArray();
    void clearColorArray();

    void setStaticColor(uint32_t color);
    //void setColorByArray();
};

#endif // LEDCOLORCONTROLLER_H
