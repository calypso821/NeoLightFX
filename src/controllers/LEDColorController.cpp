#include <cstring>

#include "controllers/LEDColorController.h"


LEDColorController::LEDColorController(int ledNum_width, int ledNum_height, bool showBottom)
    : m_frameProcessor(nullptr),
    m_ledNum_width(ledNum_width),
    m_ledNum_height(ledNum_height),
    m_showBottom(showBottom),
    m_brightness(100)
{
    // Init color array (allocate memory)
    initColorArray();
    clearColorArray();
}

// Destructor (invoked when object is destryed)
LEDColorController::~LEDColorController()
{
    // Free the allocated memory
    delete[] m_pColorArray;
}

void LEDColorController::initializeFrameProcessor(int width, int height)
{
    // INIT ImageProcessor
    m_frameProcessor = new FrameProcessor(m_ledNum_width, m_ledNum_height, m_showBottom, width, height);
}

void LEDColorController::initColorArray()
{
    int size = 2 * m_ledNum_height + m_ledNum_width;
    if (m_showBottom)
    {
        size += m_ledNum_width;
    }

    // Allocate color array memory
    m_pColorArray = new uint32_t[size];

    // Set array size
    m_colorArraySize = size;
}
    
void LEDColorController::clearColorArray()
{
    memset(m_pColorArray, 0, sizeof(uint32_t) * m_colorArraySize);
}

uint32_t* LEDColorController::getColorArray()
{
    return m_pColorArray;
}

int LEDColorController::getColorArraySize()
{
    return m_colorArraySize;
}
int LEDColorController::getLedCount()
{
    return getColorArraySize();
}
bool LEDColorController::getBottomStatus()
{
    return m_showBottom;
}
std::pair<int, int> LEDColorController::getLedResolution()
{
    return { m_ledNum_width, m_ledNum_height };
}

void LEDColorController::setStaticColor(uint32_t color)
{
    for (int i = 0; i < m_colorArraySize; ++i)
    {
        m_pColorArray[i] = color;
    }
}

void LEDColorController::setColorBySource(cv::Mat frame)
{
    m_frameProcessor->processFrame(m_pColorArray, frame);
}

//void LEDColorController::applyBrightness(bool greyscaleCorrection)
//{
//    uint32_t newColor;
//    for (int i = 0; i < m_colorArraySize; ++i)
//    {
//        newColor = m_pColorArray[i];
//        if (greyscaleCorrection) {
//            newColor = applyGreyscaleCorrection(newColor);
//        }
//        newColor = applyBrightnessCorrection(newColor);
//        m_pColorArray[i] = newColor;
//    }
//}

void LEDColorController::setBrightness(int value)
{
    m_brightness = std::clamp(value, 0, 100);
}