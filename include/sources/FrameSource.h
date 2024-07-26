#ifndef FRAMESOURCE_H
#define FRAMESOURCE_H

#include <utility>

#include <opencv2/opencv.hpp>

class FrameSource {
public:
    virtual ~FrameSource() = default;
    virtual bool getNextFrame(cv::Mat& frame) = 0;

    std::string toString() const {
        std::ostringstream oss;
        auto resolution = getResolution();
        oss << "Resolution: " << resolution.first << "x" << resolution.second
            << std::endl << "FPS: " << getFPS();
        return oss.str();
    }

    virtual std::pair<int, int> getResolution() const = 0;
    virtual float getFPS() const = 0;
};

#endif // FRAMESOURCE_H
