#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <opencv2/opencv.hpp>
#include <cstdint>

class ImageProcessor {
public:
    ImageProcessor(int ledNum_width, int ledNum_height, bool botttom);
    void init(int width, int height);
    void processFrame(uint32_t* colorArray, cv::Mat frame);

private:
    static const float HEIGHT_PATCH_RATIO;  // % of height
    static const float WIDTH_PATCH_RATIO;   // % of width
    static const float BLACK_BAR_RATIO;     // % of height

    int m_ledNum_width;
    int m_ledNum_height;
    int m_bottom;

    int m_width;
    int m_height;

    int m_verticalPatch_width;
    int m_verticalPatch_height;

    int m_horizontalPatch_width;
    int m_horizontalPatch_height;

    int m_blackBar_height;
    int m_blackBarOffset;
    int m_transitionSpeed;

    int getMeanBlack(cv::Mat blackBar);
    bool detectBlackBars(cv::Mat blackBar_top, cv::Mat blackBar_bot);
    void processBlackBars(cv::Mat frame);

    //int getMean(int oldColor, cv::Scalar avg, int num);
    uint32_t toUint32Color(cv::Scalar color);
    void processHorizontal(uint32_t* colorArray, cv::Mat frame);
    void processVertical(uint32_t* colorArray, cv::Mat frame);

};

#endif // IMAGEPROCESSOR_H