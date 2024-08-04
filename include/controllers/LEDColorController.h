#ifndef LEDCOLORCONTROLLER_H
#define LEDCOLORCONTROLLER_H

#include <cstdint>

#include <opencv2/core/mat.hpp>

#include "hardware/LEDStripConfig.h"
#include "FrameProcessor.h"

class LEDColorController 
{
public:
    LEDColorController(const LEDStripConfig& stripConfig);
    ~LEDColorController();

    uint32_t* getColorArray();
    const LEDStripConfig& getStripConfig();

    void setStaticColor(uint32_t color);
    void setBrightness(int value);
    void clearColorArray();

    //void applyBrightness(bool greyscaleCorrection);

private:
    const LEDStripConfig& m_stripConfig;

    int m_brightness;

    uint32_t* m_pColorArray;

    void initColorArray();
};

#endif // LEDCOLORCONTROLLER_H
