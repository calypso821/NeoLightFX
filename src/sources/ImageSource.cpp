#include "sources/ImageSource.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>

ImageSource::ImageSource(const std::string& filename) : frameReturned(false) {
    // Attempt to read the image using OpenCV
    image = cv::imread(filename);
    if (image.empty()) {
        std::cerr << "Error: Could not open image file: " << filename << std::endl;
        throw std::runtime_error("Error opening image file");
    }
    std::cout << "Image initialization: Success" << std::endl;
}

bool ImageSource::getNextFrame(cv::Mat& frame) {
    // Frame already returned
    if (frameReturned) {
        return false;
    }
    // Set frame to image (first time only)
    frame = image.clone();
    frameReturned = true;
    return true;
}
