#ifndef IMAGESOURCE_H
#define IMAGESOURCE_H

#include "sources/FrameSource.h"

#include <string>

#include <opencv2/opencv.hpp>

class ImageSource : public FrameSource {
public:
    ImageSource(const std::string& filename);

    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;
    float getFPS() const override;

private:
    cv::Mat image;
    bool frameCloned;
};

#endif // IMAGESOURCE_H
