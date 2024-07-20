#ifndef VIDEOSOURCE_H
#define VIDEOSOURCE_H

#include "sources/FrameSource.h"
#include <opencv2/opencv.hpp>
#include <string>

class VideoSource : public FrameSource {
public:
    // Initialize with capture card
    bool initCaptureCard(int device);

    // Initialize with video file
    bool initVideoFile(const std::string& filename);

    bool getNextFrame(cv::Mat& frame) override;

private:
    cv::VideoCapture cap;
};

#endif // VIDEOSOURCE_H
