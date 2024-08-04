#include "FrameProcessor.h"

#include "utils/color_processing_utils.h"

// Define the static constants
const float FrameProcessor::HEIGHT_PATCH_RATIO = 0.3f; // 30% of height
const float FrameProcessor::WIDTH_PATCH_RATIO = 0.2f;  // 30% of width
const float FrameProcessor::BLACK_BAR_RATIO = 0.125f;   // 10% of height
const bool FrameProcessor::DETECT_BLACK_BARS = true;   // automatically deteck black bars

FrameProcessor::FrameProcessor(const LEDStripConfig& stripConfig, int width, int height)
    : m_stripConfig(stripConfig)
{
    // Default init
    initialize(width, height);
}

void FrameProcessor::initialize(int width, int height)
{
    m_width = width;
    m_height = height;

    // On Left, Right
    m_verticalPatch_width = static_cast<int>(width * WIDTH_PATCH_RATIO);
    m_verticalPatch_height = static_cast<int>(height / m_stripConfig.height);

    // On Top, Bottom
    m_horizontalPatch_width = static_cast<int>(width / m_stripConfig.width);
    m_horizontalPatch_height = static_cast<int>(height * HEIGHT_PATCH_RATIO);

    m_blackBar_height = static_cast<int>(height * BLACK_BAR_RATIO);
    m_blackBarOffset = 0;
    m_transitionSpeed = 100;
}

void FrameProcessor::processFrame(uint32_t* colorArray, cv::Mat frame, double dt)
{
    std::cout << "Seconds elapsed: " << dt << std::endl;
    if (DETECT_BLACK_BARS) {
        processBlackBars(frame);
    }

    processHorizontal(colorArray, frame, dt);
    processVertical(colorArray, frame, dt);
}

void FrameProcessor::setTransitionSpeed(int value)
{
    m_transitionSpeed = std::clamp(value, 0, 100);
}

int FrameProcessor::getMeanBlack(cv::Mat blackBar)
{
    cv::Scalar meanBackBar = cv::mean(blackBar);
    return static_cast<int>((meanBackBar[0] + meanBackBar[1] + meanBackBar[2]) / 3);
}

bool FrameProcessor::detectBlackBars(cv::Mat blackBar_top, cv::Mat blackBar_bot)
{
    int blackBarMean_top = FrameProcessor::getMeanBlack(blackBar_top);
    int blackBarMean_bot = FrameProcessor::getMeanBlack(blackBar_bot);

    return (blackBarMean_top < 6 && blackBarMean_bot < 6);
}

void FrameProcessor::processBlackBars(cv::Mat frame) {
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
//int FrameProcessor::getMean(int old_color, cv::Scalar avg, int num) {
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
void FrameProcessor::setNewColor(uint32_t* colorArray, int index, uint32_t newColor, double dt)
{
    // Factor = 7.5
    // 60 FPS -> 0.016667 * 7.5 * 40 = 5 (transition speed)
    // 30 FPS -> 0.333333 * 7.5 * 40 = 10 (transition speed) 
    // TODO: 100 --> 100 (transition speed)
    // 7.5f = constant factor 
    // 40 = 40% of transition speed (set to m_transitionSpeed)
    float tSpeed = dt * 7.5f * 40;
    //float tSpeed = 5;
    // 1. Greyscale correction
    newColor = applyGreyscaleCorrection(newColor);
    //std::cout << "Grayscale: " << uint32ToString(newColor) << std::endl;
    // 2. Apply tranistion correction
    newColor = applyTransitionCorrection(colorArray[index], newColor, tSpeed);
    //std::cout << "Transition: " << uint32ToString(newColor) << std::endl;
    colorArray[index] = newColor;
}

uint32_t FrameProcessor::toUint32Color(cv::Scalar color)
{
    uint8_t red = static_cast<uint8_t>(color[2]);
    uint8_t green = static_cast<uint8_t>(color[1]);
    uint8_t blue = static_cast<uint8_t>(color[0]);

    // TODO: How this works? 
    return (red << 16) | (green << 8) | blue;
}

void FrameProcessor::processHorizontal(uint32_t* array, cv::Mat frame, double dt)
{
    int patch_width = m_horizontalPatch_width;
    int patch_height = m_horizontalPatch_height;

    for (int i = 0; i < m_stripConfig.width; i++)
    {
        // Top side (Left -> Right)
        cv::Mat patch_top = frame(cv::Rect(
            i * patch_width,
            0 + m_blackBarOffset,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_top = cv::mean(patch_top);
        int pos_t = i + m_stripConfig.height;
        setNewColor(array, pos_t, toUint32Color(meanColor_top), dt);

        if (m_stripConfig.bottom)
        {
            // Bottom side (Left <- Right)
            int i_inverse = m_stripConfig.width - 1 - i;
            cv::Mat patch_bot = frame(cv::Rect(
                i_inverse * patch_width,
                m_height - patch_height - m_blackBarOffset,
                patch_width,
                patch_height
            ));
            cv::Scalar meanColor_bot = cv::mean(patch_bot);
            int pos_b = i + m_stripConfig.height * 2 + m_stripConfig.width;
            setNewColor(array, pos_b, toUint32Color(meanColor_bot), dt);
        }
    }
}

void FrameProcessor::processVertical(uint32_t* array, cv::Mat frame, double dt)
{
    int patch_width = m_verticalPatch_width;
    int patch_height = m_verticalPatch_height;

    for (int i = 0; i < m_stripConfig.height; i++)
    {
        // Left side (Bottom -> Top)
        int i_inverse = m_stripConfig.height - 1 - i;
        cv::Mat patch_left = frame(cv::Rect(
            0,
            i_inverse * patch_height,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_left = cv::mean(patch_left);
        int pos_l = i;
        setNewColor(array, pos_l, toUint32Color(meanColor_left), dt);


        // Right side (Top -> Bottom)
        cv::Mat patch_right = frame(cv::Rect(
            m_width - patch_width,
            i * patch_height,
            patch_width,
            patch_height
        ));
        cv::Scalar meanColor_right = cv::mean(patch_right);
        int pos_r = i + m_stripConfig.height + m_stripConfig.width;
        setNewColor(array, pos_r, toUint32Color(meanColor_right), dt);
    }
}