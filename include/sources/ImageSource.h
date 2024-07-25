#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "sources/FrameSource.h"
#include <opencv2/opencv.hpp>
#include <string>
#include <stdexcept>

class ImageSource : public FrameSource {
public:
    ImageSource(const std::string& filename);
    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;
    float getFPS() override;

private:
    cv::Mat image;
    bool frameReturned;
};

#endif // IMAGESOURCE_H
