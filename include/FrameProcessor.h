#ifndef FRAMEPROCESSOR_H
#define FRAMEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <ws2811.h>

class FrameProcessor {
public:
    FrameProcessor(int width, int height);
    void call(cv::Mat frame, ws2811_led_t* led_array);

private:
    int width;
    int height;
    int patch_width;
    int patch_height;
    int patch_width_length;
    int patch_height_length;
    int padding;
    int black_bars_height;
    int averaging;

    int getColor(cv::Scalar avg);
    bool getMeanBlack(cv::Scalar black_top, cv::Scalar black_bot);
    int getMean(int old_color, cv::Scalar avg, int num);
    void processBlackBars(cv::Mat frame);
    void processHorizontal(ws2811_led_t* array, cv::Mat frame);
    void processVertical(ws2811_led_t* array, cv::Mat frame);
};

#endif // FRAMEPROCESSOR_H
