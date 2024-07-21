#include "sources/VideoSource.h"
#include <stdexcept>
#include <iostream>

bool VideoSource::initCaptureCard(int device) {
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

    // Verify properties
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    std::cout << "Set resolution to 640x480 and FPS to 30" << std::endl;
    std::cout << "Actual resolution: " << width << "x" << height << std::endl;
    std::cout << "Actual FPS: " << fps << std::endl;

    std::cout << "Video initialization: Success" << std::endl;
    return true;
}

bool VideoSource::initVideoFile(const std::string& filename) 
{
    if (filename.empty()) {
        std::cerr << "Error: The filename is empty." << std::endl;
        throw std::runtime_error("Filename cannot be empty");
    }

    cap.open(filename);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error opening video file");
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 60);

    // Verify properties
    double width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(cv::CAP_PROP_FPS);

    std::cout << "Set resolution to 640x480 and FPS to 30" << std::endl;
    std::cout << "Actual resolution: " << width << "x" << height << std::endl;
    std::cout << "Actual FPS: " << fps << std::endl;

    std::cout << "Video initialization: Success" << std::endl;
    return true;
}

std::pair<int, int> VideoSource::getResolution() const {
    int width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    return { width, height };
}

bool VideoSource::getNextFrame(cv::Mat& frame) {
    cap >> frame;
    return !frame.empty();
}
