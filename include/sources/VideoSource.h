#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include "sources/FrameSource.h"

class VideoSource : public FrameSource {
public:
    VideoSource(const std::string& filename);

    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;
    float getFPS() const override;

private:
    cv::VideoCapture cap;
};

#endif // VIDEOSOURCE_H
