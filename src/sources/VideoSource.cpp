#include "sources/VideoSource.h"

#include <stdexcept>

VideoSource::VideoSource(const std::string& filename) 
{
    if (filename.empty()) {
        std::cerr << "Error: The filename is empty." << std::endl;
        throw std::runtime_error("Filename cannot be empty");
    }

    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    cap.open(filename);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error opening video file");
    }
 
    std::cout << "Video source initialization: Success" << std::endl;
    std::cout << this->toString() << std::endl;
}

std::pair<int, int> VideoSource::getResolution() const
{
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return { width, height };
}

float VideoSource::getFPS() const
{
    return static_cast<float>(cap.get(cv::CAP_PROP_FPS));
}

bool VideoSource::getNextFrame(cv::Mat& frame) {
    cap >> frame;
    return !frame.empty();
}


