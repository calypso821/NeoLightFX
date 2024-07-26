#ifndef CAPTURESOURCE_H
#define CAPTURESOURCE_H

#include "sources/FrameSource.h"

#include <string>

#include <opencv2/opencv.hpp>

class CaptureSource : public FrameSource {
public:
    CaptureSource(int device);

    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;
    float getFPS() const override;

private:
    cv::VideoCapture cap;
};

#endif // CAPTURESOURCE_H
