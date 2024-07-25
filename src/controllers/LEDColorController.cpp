#include <cstring>

#include "controllers/LEDColorController.h"


LEDColorController::LEDColorController(int ledNum_width, int ledNum_height, bool showBottom)
    : m_frameProcessor(ledNum_width, ledNum_height, showBottom),
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

void LEDColorController::initFrameProcessor(int width, int height)
{
    // INIT ImageProcessor
    //m_frameProcessor = ImageProcessor(m_ledNum_width, m_ledNum_height, m_showBottom);
    m_frameProcessor.init(width, height);
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
bool LEDColorController::getBottomStatus()
{
    return m_showBottom;
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
    m_frameProcessor.processFrame(m_pColorArray, frame);
}

//void LEDColorController::setColorByName(Color color)
//{
//    setStaticColor(static_cast<uint32_t>(color));
//}
//
//void LEDColorController::setColorByHexCode(uint32_t color)
//{
//    setStaticColor(color);
//}