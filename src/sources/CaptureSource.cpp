#include "sources/CaptureSource.h"


CaptureSource::CaptureSource(int device)
{
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);
    std::cout << "Opening device..." << std::endl;
    cap.open(device);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error opening capture card");
    }
    std::cout << "Device opened: Success" << std::endl;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
    cap.set(cv::CAP_PROP_FPS, 60);
}

std::pair<int, int> CaptureSource::getResolution() const
{
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return { width, height };
}

float CaptureSource::getFPS() const
{
    return static_cast<float>(cap.get(cv::CAP_PROP_FPS));
}

bool CaptureSource::getNextFrame(cv::Mat& frame) {
    cap >> frame;
    return !frame.empty();
}