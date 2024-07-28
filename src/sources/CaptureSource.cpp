#include "sources/CaptureSource.h"


CaptureSource::CaptureSource(int device)
{
    openDevice(device);
    setProperties(640, 480, 66);
    std::cout << "Capture source initialization: Success" << std::endl;
    std::cout << this->toString() << std::endl;
}

CaptureSource::CaptureSource(int device, int width, int height, int fps)
{
    openDevice(device);
    setProperties(width, height, fps);
    std::cout << "Capture source initialization: Success" << std::endl;
    std::cout << this->toString() << std::endl;
}
void CaptureSource::openDevice(int device)
{
    std::cout << "Opening capture device..." << std::endl;
    //std::string pipeline = "v4l2src ! videoconvert ! appsink";
    //cap.open(pipeline, cv::CAP_GSTREAMER);

    cap.open(device, cv::CAP_V4L2);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error opening capture card");
    }
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

void CaptureSource::setProperties(int width, int height, int fps)
{
    if (!cap.isOpened()) {
        throw std::runtime_error("Error setting capture device properties");
    }
    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);
    cap.set(cv::CAP_PROP_FPS, fps);
}