#include <iostream>

#include "LEDStripControllerRpi3.h"



void LEDStripControllerRpi3::init() {
    // Implementation specific to rpi3
    std::cout << "Initializing LED strip for RPi3" << std::endl;
}

void LEDStripControllerRpi3::clear() {
    // Implementation specific to rpi3
    std::cout << "Clearing LED strip for RPi3" << std::endl;
}

void LEDStripControllerRpi3::render() {
    // Implementation specific to rpi3
    std::cout << "Rendering LED strip for RPi3" << std::endl;
}


//// WS2811 definitions
//#define TARGET_FREQ             800000
//#define GPIO_PIN                18
//#define DMA                     10
//#define STRIP_TYPE              WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds
//
//// LED definitions
//#define BOTTOM                  false   // include lower side
//
//#define LED_HEIGHT              20      // number of leds (vertical)
//#define LED_WIDTH               34      // number of leds (horizontal)
//#define LED_COUNT               (LED_HEIGHT * 2 + LED_WIDTH + LED_WIDTH * BOTTOM)
//
//#define black_bars_size         0.125   // 12.5% of height
//#define h_patch_size            0.3     // 30% of height
//#define w_patch_size            0.2     // 20% of width
//
//ws2811_t ledstring;
//ws2811_return_t ret;
//
//// ----------- INIT LED STRIP ------------ //
//ws2811_return_t ledstring_init(uint8_t brightness) {
//    ledstring =
//    {
//        .freq = TARGET_FREQ,
//        .dmanum = DMA,
//        .channel =
//        {
//            [0] =
//            {
//                .gpionum = GPIO_PIN,
//                .invert = 0,
//                .count = LED_COUNT,
//                .strip_type = STRIP_TYPE,
//                .brightness = brightness,
//            },
//            [1] =
//            {
//                .gpionum = 0,
//                .invert = 0,
//                .count = 0,
//                .brightness = 0,
//            },
//        },
//    };
//    ret = ws2811_init(&ledstring);
//
//    if (ret != WS2811_SUCCESS) {
//        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
//    }
//    else {
//        fprintf(stderr, "ws2811_init: %s\n", ws2811_get_return_t_str(ret));
//    }
//    return ret;
//}
//// ----------- INIT ARRAY ------------ //
//ws2811_led_t* array_init() {
//    // led array initialize
//    return static_cast<ws2811_led_t*>(malloc(sizeof(ws2811_led_t) * LED_COUNT));
//}
//
//// ----------- RENDER ARRAY ------------ //
//void array_render(ws2811_led_t* array) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        ledstring.channel[0].leds[x] = array[x];
//    }
//}
//
//// ----------- CLEAR ARRAY ------------ //
//void array_clear(ws2811_led_t* array) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        array[x] = 0;
//    }
//}
//
//// ----------- ARRAY SET COLOR ------------ //
//void array_setStaticColor(ws2811_led_t* array, uint32_t color) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        array[x] = color;
//    }
//}
