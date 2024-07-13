#include "VideoController.h"
#include <iostream>

VideoController::VideoController(std::string source, LEDColorController* ledColorController) {
    this->source = source;
    this->ledColorController = ledColorController;
    cap.open(source);

    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream or file" << std::endl;
    } else {
        std::cout << "Video Capture init - success" << std::endl;
    }
}

void VideoController::start() {
    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        ledColorController->processFrame(frame);
    }
}
