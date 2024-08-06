#ifndef EFFECT_CONTROLLER_H
#define EFFECT_CONTROLLER_H

#include "Effects.h"
#include "hardware/LEDStripConfig.h"

enum class EffectType
{
    RAINBOW,
    PROGRESS
};

class EffectController
{
public:
    EffectController(const LEDStripConfig& stripConfig);
    ~EffectController();  // Destructor to clean up the allocated memory

    void setEffect(EffectType effect);
    void applyEffect(uint32_t* p_colorArray, float progress);

private:
    Effect* m_currentEffect = nullptr;  
    const LEDStripConfig& m_stripConfig;
};

#endif // EFFECT_CONTROLLER_H
