#include <iostream>

#include "LEDStripController.h"
#include "LEDStripControllerRpi3.h"

#include "ws2811.h"

// WS2811 definitions
#define TARGET_FREQ             800000
#define GPIO_PIN                18
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds

// LED definitions
#define BOTTOM                  false   // include lower side
  
#define LED_HEIGHT              20      // number of leds (vertical)
#define LED_WIDTH               34      // number of leds (horizontal)
#define LED_COUNT               (LED_HEIGHT * 2 + LED_WIDTH + LED_WIDTH * BOTTOM)


int main()
{
    
    ws2811_t ledstring =
    {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel =
        {
            [0] =
            {
                .gpionum = GPIO_PIN,
                .invert = 0,
                .count = LED_COUNT,
                .strip_type = STRIP_TYPE,
                .brightness = 100,
            },
            [1] =
            {
                .gpionum = 0,
                .invert = 0,
                .count = 0,
                .brightness = 0,
            },
        },
    };
    
    ws2811_return_t ret = ws2811_init(&ledstring);
    
    if (ret != WS2811_SUCCESS) {
        std::cerr << "ws2811_init failed: " << ws2811_get_return_t_str(ret) << std::endl;
    } else {
        std::cerr << "ws2811_init: " << ws2811_get_return_t_str(ret) << std::endl;
    }
    
    LEDStripController* controller;

    controller = new LEDStripControllerRpi3();

    controller->init();
    controller->clear();
    controller->render();


    return 0;
}
