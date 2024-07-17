#include <iostream>

#include "LEDStripController.h"
#include "LEDStripControllerRpi3.h"

int main()
{
    LEDStripController* controller;

    controller = new LEDStripControllerRpi3();

    controller->init();
    controller->clear();
    controller->render();


    return 0;
}
