#include "controllers/EffectController.h"

#include "Effects.h"

EffectController::EffectController(const LEDStripConfig& stripConfig)
    : m_stripConfig(stripConfig)
{

}
EffectController::~EffectController()
{
    delete m_currentEffect;
}

void EffectController::setEffect(EffectType effect)
{
    if (m_currentEffect)
    {
        delete m_currentEffect;
    }

    switch (effect)
    {
    case EffectType::RAINBOW:
        m_currentEffect = new RainbowEffect{ m_stripConfig };
        break;
    case EffectType::PROGRESS:
        m_currentEffect = new ProgressEffect{ m_stripConfig };
        break;
    default:
        m_currentEffect = nullptr;
        break;
    }
}

void EffectController::applyEffect(uint32_t* p_colorArray, float progress)
{
    if (m_currentEffect)
    {
        m_currentEffect->apply(p_colorArray, progress);
    }
}
