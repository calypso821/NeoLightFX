#ifndef VIDEOCONTROLLER_H
#define VIDEOCONTROLLER_H

#include "LEDColorController.h"
#include <opencv2/opencv.hpp>

class VideoController {
public:
    VideoController(std::string source, LEDColorController* ledColorController);
    void start();

private:
    std::string source;
    LEDColorController* ledColorController;
    cv::VideoCapture cap;
};

#endif // VIDEOCONTROLLER_H
