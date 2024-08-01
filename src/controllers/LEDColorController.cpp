#include <cstring>

#include "controllers/LEDColorController.h"


LEDColorController::LEDColorController(const LEDStripConfig& stripConfig)
    : m_stripConfig(stripConfig),
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

void LEDColorController::initColorArray()
{
    int size = 2 * m_stripConfig.ledNumHeight + m_stripConfig.ledNumWidth;
    if (m_stripConfig.showBottom)
    {
        size += m_stripConfig.ledNumWidth;
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
    return m_stripConfig.showBottom;
}
std::pair<int, int> LEDColorController::getLedResolution()
{
    return { m_stripConfig.ledNumWidth, m_stripConfig.ledNumHeight };
}

void LEDColorController::setStaticColor(uint32_t color)
{
    for (int i = 0; i < m_colorArraySize; ++i)
    {
        m_pColorArray[i] = color;
    }
}

void LEDColorController::setBrightness(int value)
{
    m_brightness = std::clamp(value, 0, 100);
}
