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
    // Allocate color array memory
    m_pColorArray = new uint32_t[m_stripConfig.count];
}
    
void LEDColorController::clearColorArray()
{
    memset(m_pColorArray, 0, sizeof(uint32_t) * m_stripConfig.count);
}

uint32_t* LEDColorController::getColorArray()
{
    return m_pColorArray;
}

const LEDStripConfig& LEDColorController::getStripConfig()
{
    return m_stripConfig;
}

void LEDColorController::setStaticColor(uint32_t color)
{
    for (int i = 0; i < m_stripConfig.count; ++i)
    {
        m_pColorArray[i] = color;
    }
}

void LEDColorController::setBrightness(int value)
{
    m_brightness = std::clamp(value, 0, 100);
}
