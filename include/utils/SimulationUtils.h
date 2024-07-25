#ifndef SIMULATION_UTILS_H
#define SIMULATION_UTILS_H

#include <cstdint>
#include <opencv2/opencv.hpp>

// Function to simulate the LED display around the frame
void simulateLEDs(const cv::Mat& frame, const uint32_t* colorArray, int ledNumWidth, int ledNumHeight, bool showBottom);

// Function to create a solid color frame
cv::Mat createSolidColorFrame(int width, int height, uint32_t color);

#endif // SIMULATION_UTILS_H
