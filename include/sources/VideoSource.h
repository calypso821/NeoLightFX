#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include <string>

#include <opencv2/opencv.hpp>

#include "sources/FrameSource.h"


class VideoSource : public FrameSource {
public:
    // Initialize with capture card
    bool initCaptureCard(int device);

    // Initialize with video file
    bool initVideoFile(const std::string& filename);

    bool getNextFrame(cv::Mat& frame) override;
    std::pair<int, int> getResolution() const override;

private:
    cv::VideoCapture cap;
};

#endif // VIDEOSOURCE_H
