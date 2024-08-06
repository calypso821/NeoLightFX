#ifndef EFFECTS_H
#define EFFECTS_H

#include <cstdint>
#include <cstddef>
#include <cmath>

#include "hardware/LEDStripConfig.h"

// Declare the color array and its size
extern uint32_t rainbowColors[];
extern const size_t rainbowColorsSize;

// Interface for effects
class Effect
{
public:
    Effect(const LEDStripConfig& config) : m_stripConfig(config) {}
    virtual void apply(uint32_t* p_colorArray, float progress) = 0;
    virtual ~Effect() = default;

protected:
    const LEDStripConfig& m_stripConfig;
};

// Rainbow Effect Implementation
class RainbowEffect : public Effect
{
public:
    RainbowEffect(const LEDStripConfig& config) : Effect(config) {}
    void apply(uint32_t* p_colorArray, float progress) override;
};

// Progress Effect Implementation
class ProgressEffect : public Effect
{
public:
    ProgressEffect(const LEDStripConfig& config) : Effect(config) {}
    void apply(uint32_t* p_colorArray, float progress) override;
};

#endif // EFFECTS_H
