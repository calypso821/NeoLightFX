#include "sources/VideoSource.h"
#include "sources/ImageSource.h"
#include "controllers/InputSourceController.h"
#include "controllers/LEDColorController.h"
//#include "processors/ImageProcessor.h"
#include <iostream>
#include <filesystem>
#include <opencv2/opencv.hpp>

// Function to check available capture devices
void checkAvailableDevices(int maxDevices = 10) {
    for (int device = 0; device < maxDevices; ++device) {
        cv::VideoCapture cap(device);
        if (cap.isOpened()) {
            std::cout << "Device " << device << " is available." << std::endl;
            cap.release();
        }
        else {
            std::cout << "Device " << device << " is not available." << std::endl;
        }
    }
}

int main() {
    //checkAvailableDevices();

    LEDColorController lcc(30, 15, false);
    //ImageProcessor imageProcessor(&ledColorController);

    // Capture card source object
    //VideoSource captureCardSource;
    //if (captureCardSource.initCaptureCard(0)) { // assuming device 0
    //    InputSourceController captureCardController(&captureCardSource, &lcc);
    //    captureCardController.init();
    //    captureCardController.start();
    //}

    // Video source object
    VideoSource videoFileSource;
    bool video_init_status = videoFileSource.initVideoFile("D:/resources/videos/Ambilight.mp4");
    if (video_init_status) {
        InputSourceController videoFileController(&videoFileSource, &lcc);
        videoFileController.init();
        videoFileController.start();
    }


   // Image source object 
    //ImageSource imageFileSource("D:/resources/pictures/test.jpg");
    //InputSourceController imageFileController(&imageFileSource, &lcc);
    //imageFileController.start();

    return 0;
}
