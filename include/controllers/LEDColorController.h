#ifndef LEDCOLORCONTROLLER_H
#define LEDCOLORCONTROLLER_H

#include <cstdint>

#include <opencv2/core/mat.hpp>

#include "processors/ImageProcessor.h"

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

    void initImageProcessor(int width, int height);

    uint32_t* getColorArray();
    int getColorArraySize();

    void setColorByName(Color color);
    void setColorByHexCode(uint32_t color);
    void setColorBySource(cv::Mat frame);

private:
    int m_ledNum_width;
    int m_ledNum_height;

    int m_brightness;
    bool m_showBottom;


    ImageProcessor m_imageProcessor;

    int m_colorArraySize;
    uint32_t* m_pColorArray;

    void initColorArray();
    void clearColorArray();

    void setStaticColor(uint32_t color);
};

#endif // LEDCOLORCONTROLLER_H
