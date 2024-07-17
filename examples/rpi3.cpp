#include "LEDStripController.h"
#include "rpi3/LEDStripControllerRpi3.h"


int main()
{
    LEDStripController* controller;

    controller = new LEDStripControllerRpi3();

    controller->init();


    return 0;
}