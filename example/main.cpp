#include <iostream>
#include "LEDColorController.h"

static const int LEDS_WIDTH = 45;
static const int LEDS_HEIGHT = 18;
static const bool SHOW_BOTTOM = false;

static void printElements(uint8_t* pArr, int offset, int length)
{
    for (int i = 0; i < length; ++i)
    {
        int i_ele = offset * 3 + i * 3;
        std::cout << static_cast<int>(pArr[i_ele]) << ".";
        std::cout << static_cast<int>(pArr[i_ele + 1]) << ".";
        std::cout << static_cast<int>(pArr[i_ele + 2]) << ", ";
    }
    std::cout << std::endl;
}


static void printColorArray(uint8_t* pArr)
{
    int offset = 0;

    // Left
    std::cout << "Left: ";
    printElements(pArr, offset, LEDS_HEIGHT);

    // Top 
    offset += LEDS_HEIGHT;
    std::cout << "Top: ";
    printElements(pArr, offset, LEDS_WIDTH);

    // RIght
    offset += LEDS_WIDTH;
    std::cout << "Right: ";
    printElements(pArr, offset, LEDS_HEIGHT);

    // Bottom
    if (SHOW_BOTTOM)
    {
        offset += LEDS_HEIGHT;
        std::cout << "Bottom: ";
        printElements(pArr, offset, LEDS_WIDTH);
    }
    std::cout << std::endl;
}

int main()
{
    // Initialize LEDColorController with the desired width, height, and brightness

    LEDColorController lcc = LEDColorController(LEDS_WIDTH, LEDS_HEIGHT, SHOW_BOTTOM);
    uint8_t* pArr = lcc.getColorArray();

    lcc.setColorByHexCode(0x00FFFF);
    lcc.setColorByName(Color::Red);
    lcc.setColorByName(Color::Green);

    printColorArray(pArr);


    return 0;
}