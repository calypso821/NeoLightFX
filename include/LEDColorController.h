#ifndef LEDCOLORCONTROLLER_H
#define LEDCOLORCONTROLLER_H

#include "FrameProcessor.h"
#include "StaticColor.h"
#include <ws2811.h>
#include <opencv2/opencv.hpp>

class LEDColorController {
public:
    LEDColorController(int width, int height, int brightness);
    ~LEDColorController();
    void setColorProcessor(FrameProcessor *processor);
    void setStaticColor(StaticColor *staticColor);
    ws2811_led_t* getColorArray();
    void processFrame(cv::Mat frame);
    void setStaticColorByString(std::string color);
    void setStaticColorByHex(uint32_t color);

private:
    int width;
    int height;
    int brightness;
    ws2811_led_t* led_array;
    FrameProcessor* processor;
    StaticColor* staticColor;
    ws2811_led_t* array_init();
};

#endif // LEDCOLORCONTROLLER_H
