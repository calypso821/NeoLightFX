#include "processors/ImageProcessor.h"

//TODO: Input paramter
#define LED_WIDTH   50
#define LED_HEIGHT  20
#define DETECT_BLACK_BARS  true

// Define the static constants
const float ImageProcessor::HEIGHT_PATCH_RATIO = 0.2f; // 30% of height
const float ImageProcessor::WIDTH_PATCH_RATIO = 0.3f;  // 30% of width
const float ImageProcessor::BLACK_BAR_RATIO = 0.125f;   // 10% of height

ImageProcessor::ImageProcessor(int ledNum_width, int ledNum_height, bool botttom)
{
    m_ledNum_width = ledNum_width;
    m_ledNum_height = ledNum_height;

    m_bottom = botttom;
}

void ImageProcessor::init(int width, int height)
{
    m_width = width;
    m_height = height;

    // On Left, Right
    m_verticalPatch_width = static_cast<int>(width * WIDTH_PATCH_RATIO);
    m_verticalPatch_height = static_cast<int>(height / m_ledNum_height);

    // On Top, Bottom
    m_horizontalPatch_width = static_cast<int>(width / m_ledNum_width);
    m_horizontalPatch_height = static_cast<int>(height * HEIGHT_PATCH_RATIO);

    m_blackBar_height = static_cast<int>(height * BLACK_BAR_RATIO);
    m_blackBarOffset = 0;
    m_transitionSpeed = 1;
}

int ImageProcessor::getMeanBlack(cv::Mat blackBar)
{
    cv::Scalar meanBackBar = cv::mean(blackBar);
    return static_cast<int>((meanBackBar[0] + meanBackBar[1] + meanBackBar[2]) / 3);
}

bool ImageProcessor::detectBlackBars(cv::Mat blackBar_top, cv::Mat blackBar_bot)
{
    int blackBarMean_top = ImageProcessor::getMeanBlack(blackBar_top);
    int blackBarMean_bot = ImageProcessor::getMeanBlack(blackBar_bot);

    return (blackBarMean_top < 6 && blackBarMean_bot < 6);
}

void ImageProcessor::processBlackBars(cv::Mat frame) {
    // Top side patch 
    cv::Mat blackBar_top = frame(cv::Rect(
        0, 
        0,
        m_width,
        m_blackBar_height
    ));
    // Bottom side patch
    cv::Mat blackBar_bot = frame(cv::Rect(
        0,
        m_height - m_blackBar_height,
        m_width,
        m_blackBar_height
    ));

    if (detectBlackBars(blackBar_top, blackBar_bot))
    {
        m_blackBarOffset = m_blackBar_height;
        int new_height = m_height - 2 * m_blackBar_height;
        m_horizontalPatch_height = static_cast<int>(new_height * HEIGHT_PATCH_RATIO);
    }
    else 
    {
        m_blackBarOffset = 0;
        m_horizontalPatch_height = static_cast<int>(m_height * HEIGHT_PATCH_RATIO);
    }
}

// TODO: Transition
//int ImageProcessor::getMean(int old_color, cv::Scalar avg, int num) {
//    int red_n = static_cast<int>(avg[2]);
//    int green_n = static_cast<int>(avg[1]);
//    int blue_n = static_cast<int>(avg[0]);
//
//    if (num != 0) {
//        int red_o = old_color >> 16 & 0xFF;
//        int green_o = old_color >> 8 & 0xFF;
//        int blue_o = old_color & 0xFF;
//
//        for (int i = 0; i < num; i++) {
//            red_n = (red_o + red_n) / 2;
//            green_n = (green_o + green_n) / 2;
//            blue_n = (blue_o + blue_n) / 2;
//        }
//    }
//    return (red_n << 16) + (green_n << 8) + blue_n;
//}

uint32_t ImageProcessor::toUint32Color(cv::Scalar color)
{
    uint8_t red = static_cast<uint8_t>(color[2]);
    uint8_t green = static_cast<uint8_t>(color[1]);
    uint8_t blue = static_cast<uint8_t>(color[0]);

    // TODO: How this works? 
    return (red << 16) | (green << 8) | blue;
}

void ImageProcessor::processHorizontal(uint32_t* array, cv::Mat frame)
{
    int patch_width = m_horizontalPatch_width;
    int patch_height = m_horizontalPatch_height;

    for (int i = 0; i < m_ledNum_width; i++)
    {
        // Top side (Left -> Right)
        cv::Mat patch_top = frame(cv::Rect(
            i * patch_width,
            0 + m_blackBarOffset,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_top = cv::mean(patch_top);
        int pos_t = m_ledNum_height;
        //array[pos_t + i] = getMean(array[pos_t + i], avg_top, averaging);
        array[pos_t + i] = toUint32Color(meanColor_top);

        if (m_bottom)
        {
            // Bottom side (Right -> Left)
            int i_inverse = m_ledNum_width - 1 - i;
            cv::Mat patch_bot = frame(cv::Rect(
                i_inverse * patch_width,
                m_height - patch_height - m_blackBarOffset,
                patch_width,
                patch_height
            ));
            cv::Scalar meanColor_bot = cv::mean(patch_top);
            int pos_b = m_ledNum_height * 2 + m_ledNum_width;
            //array[pos_b + i] = getMean(array[pos_b + i], avg_bot, averaging);
            array[pos_b + i] = toUint32Color(meanColor_bot);
        }
    }
}

void ImageProcessor::processVertical(uint32_t* array, cv::Mat frame)
{
    int patch_width = m_verticalPatch_width;
    int patch_height = m_verticalPatch_height;

    for (int i = 0; i < m_ledNum_height; i++)
    {
        // Left side (Bottom -> Top)
        int i_inverse = m_ledNum_height - 1 - i;
        cv::Mat patch_left = frame(cv::Rect(
            0,
            i_inverse * patch_height,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_left = cv::mean(patch_left);
        int pos_l = 0;
        // TODO: Transition smooth
        //array[pos_l + i] = getMean(array[pos_l + i], avg_left, averaging);
        array[pos_l + i] = toUint32Color(meanColor_left);

        // Right side (Top -> Bottom)
        cv::Mat patch_right = frame(cv::Rect(
            m_width - patch_width,
            i * patch_height,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_right = cv::mean(patch_right);
        int pos_r = m_ledNum_height + m_ledNum_width;
        //array[pos_r + i] = getMean(array[pos_r + i], avg_right, averaging);
        array[pos_r + i] = toUint32Color(meanColor_right);
    }
}

void ImageProcessor::processFrame(uint32_t* colorArray, cv::Mat frame)
{
    std::cout << "Processing frame..." << std::endl;
    if (DETECT_BLACK_BARS) {
        processBlackBars(frame);
    }

    processHorizontal(colorArray, frame);
    processVertical(colorArray, frame);
}
