#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <ws2811.h>

class ImageProcessor {
public:
    ImageProcessor(int width, int height);
    void call(uint32_t* colorArray, cv::Mat source);

private:
    static const float HEIGHT_PATCH_RATIO;   // % of height
    static const float WIDTH_PATCH_RATIO;    // % of width
    static const float BLACK_BAR_RATIO;     // % of height

    int m_width;
    int m_height;
    int patch_width;
    int patch_height;
    int patch_width_length;
    int patch_height_length;
    int padding;
    int black_bars_height;
    int transitionSpeed;

    int getMeanBlack(cv::Mat blackBar);
    bool detectBlackBars(cv::Mat blackBar_top, cv::Mat blackBar_bot);
    void processBlackBars(cv::Mat source);

    int getColor(cv::Scalar avg);
    int getMean(int oldColor, cv::Scalar avg, int num);
    uint32_t toUint32Color(cv::Scalar color);
    void processHorizontal(uint32_t* colorArray, cv::Mat source);
    void processVertical(uint32_t* colorArray, cv::Mat source);

};

#endif // IMAGEPROCESSOR_H