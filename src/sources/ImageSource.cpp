#include "sources/ImageSource.h"
#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <fstream>

ImageSource::ImageSource(const std::string& filename)
    : frameCloned(false)
{
    image = cv::imread(filename);
    if (image.empty()) {
        throw std::runtime_error("Error opening image file");
    }
    std::cout << "Image source initialization: Success" << std::endl;
    std::cout << this->toString() << std::endl;
}

std::pair<int, int> ImageSource::getResolution() const
{
    int width = static_cast<int>(image.cols);
    int height = static_cast<int>(image.rows);
    return { width, height };
}

float ImageSource::getFPS() const
{
    return 10.0f;
}

bool ImageSource::getNextFrame(cv::Mat& frame)
{
    // Frame already returned
    if (!frameCloned) {
        frame = image.clone();
        frameCloned = true;
    }
    return true;
}
