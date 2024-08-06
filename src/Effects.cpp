#include "Effects.h"
#include "utils/color_processing_utils.h"

uint32_t rainbowColors[] = {
    0xFF0000, // Red
    0xFFA500, // Orange
    0xFFFF00, // Yellow
    0x00FF00, // Green
    0x0000FF, // Blue
    0x4B0082, // Indigo
    0xEE82EE  // Violet
};

// Optionally, you can define the size of the array
const size_t rainbowColorsSize = sizeof(rainbowColors) / sizeof(rainbowColors[0]);

void RainbowEffect::apply(uint32_t* p_colorArray, float progress)
{
    for (size_t i = 0; i < m_stripConfig.count; ++i)
    {
        float ledPosition = static_cast<float>(i) / m_stripConfig.count;
        float colorPosition = fmod(progress + ledPosition, 1.0f) * rainbowColorsSize;

        size_t colorIndex1 = static_cast<size_t>(colorPosition);
        size_t colorIndex2 = (colorIndex1 + 1) % rainbowColorsSize;

        float t = colorPosition - colorIndex1;

        p_colorArray[i] = lerpColor(rainbowColors[colorIndex1], rainbowColors[colorIndex2], t);
    }
}

void ProgressEffect::apply(uint32_t* p_colorArray, float progress)
{
    static int prog = 0;
    static int lengthProg = m_stripConfig.count / 2;

    if (prog <= lengthProg)
    {
        for (size_t i = 0; i <= lengthProg; i++)
        {
            p_colorArray[i] = 0;
            p_colorArray[m_stripConfig.count - 1 - i] = 0;

            p_colorArray[prog] = 0xFFFFFF;
            p_colorArray[m_stripConfig.count - 1 - prog] = 0xFFFFFF;
        }
        prog++;
    }
    else
    {
        lengthProg--;
        prog = 0;
    }
}
