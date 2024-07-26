#ifndef LEDCOLORCONTROLLER_H
#define LEDCOLORCONTROLLER_H

#include <cstdint>

#include <opencv2/core/mat.hpp>

#include "FrameProcessor.h"

class LEDColorController 
{
public:
    LEDColorController(int ledNum_width, int ledNum_height, bool showBotttom);
    ~LEDColorController();

    void initFrameProcessor(int width, int height);

    uint32_t* getColorArray();
    int getColorArraySize();
    bool getBottomStatus();
    std::pair<int, int> getLedResolution();

    void setStaticColor(uint32_t color);
    void setColorBySource(cv::Mat frame);

    //void setColorByName(Color color);
    //void setColorByHexCode(uint32_t color);

private:
    int m_ledNum_width;
    int m_ledNum_height;

    int m_brightness;
    bool m_showBottom;


    FrameProcessor m_frameProcessor;

    int m_colorArraySize;
    uint32_t* m_pColorArray;

    void initColorArray();
    void clearColorArray();
};

#endif // LEDCOLORCONTROLLER_H
