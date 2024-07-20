#ifndef FRAMESOURCE_H
#define FRAMESOURCE_H

#include <opencv2/opencv.hpp>

class FrameSource {
public:
    virtual ~FrameSource() = default;
    virtual bool getNextFrame(cv::Mat& frame) = 0;
};

#endif // FRAMESOURCE_H
