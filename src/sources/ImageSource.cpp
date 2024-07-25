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

std::pair<int, int> ImageSource::getResolution() const
{
    int width = static_cast<int>(image.cols);
    int height = static_cast<int>(image.rows);
    return { width, height };
}

bool ImageSource::getNextFrame(cv::Mat& frame)
{
    // Frame already returned
    if (frameReturned) {
        return false;
    }
    // Set frame to image (first time only)
    frame = image.clone();
    frameReturned = true;
    return true;
}
float ImageSource::getFPS()
{
    return 5.0f;
}
