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

    void initializeFrameProcessor(int width, int height);

    uint32_t* getColorArray();
    int getColorArraySize();
    int getLedCount();
    bool getBottomStatus();
    std::pair<int, int> getLedResolution();

    void setStaticColor(uint32_t color);
    void setColorBySource(cv::Mat frame);
    void setBrightness(int value);
    void clearColorArray();

    //void applyBrightness(bool greyscaleCorrection);

private:
    int m_ledNum_width;
    int m_ledNum_height;
    bool m_showBottom;

    int m_brightness;

    FrameProcessor* m_frameProcessor;

    int m_colorArraySize;
    uint32_t* m_pColorArray;

    void initColorArray();
};

#endif // LEDCOLORCONTROLLER_H
