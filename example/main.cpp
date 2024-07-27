#include <iostream>
#include <iomanip>

#include "controllers/LEDColorController.h"

static const int LEDS_WIDTH = 45;
static const int LEDS_HEIGHT = 18;
static const bool SHOW_BOTTOM = false;

static void printElements(uint32_t* pArr, int offset, int length)
{
    for (int i = 0; i < length; ++i)
    {
        std::cout << "0x" << std::hex << std::setw(6) << std::setfill('0') << pArr[offset + i] << " ";
    }
    std::cout << std::dec << std::endl;
}


static void printColorArray(uint32_t* pArr)
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

    /*LEDColorController lcc = LEDColorController(LEDS_WIDTH, LEDS_HEIGHT, SHOW_BOTTOM);
    uint32_t* pArr = lcc.getColorArray();

    lcc.setColorByHexCode(0x00FFFF);
    lcc.setColorByName(Color::Red);
    lcc.setColorByName(Color::Green);

    printColorArray(pArr);
*/

    return 0;
}