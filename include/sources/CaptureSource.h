#ifndef CAPTURESOURCE_H
#define CAPTURESOURCE_H

#include "sources/FrameSource.h"

#include <string>

#include <opencv2/opencv.hpp>

class CaptureSource : public FrameSource {
public:
    CaptureSource(int device);
    CaptureSource(int device, int width, int height, int fps);

    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;
    float getFPS() const override;

private:
    void openDevice(int device);
    void setProperties(int width, int height, int fps);
    cv::VideoCapture cap;
};

#endif // CAPTURESOURCE_H
