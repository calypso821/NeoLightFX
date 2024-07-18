#include "LEDStripController.h"
#include <iostream>

LEDStripController::LEDStripController(int brightness) {
    ledstring.freq = TARGET_FREQ;
    ledstring.dmanum = DMA;
    ledstring.channel[0].gpionum = GPIO_PIN;
    ledstring.channel[0].invert = 0;
    ledstring.channel[0].count = LED_COUNT;
    ledstring.channel[0].strip_type = STRIP_TYPE;
    ledstring.channel[0].brightness = brightness;
    ledstring.channel[1].gpionum = 0;
    ledstring.channel[1].invert = 0;
    ledstring.channel[1].count = 0;
    ledstring.channel[1].brightness = 0;
    ret = ws2811_init(&ledstring);

    if (ret != WS2811_SUCCESS) {
        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
    } else {
        fprintf(stderr, "ws2811_init: %s\n", ws2811_get_return_t_str(ret));
    }
}

LEDStripController::~LEDStripController() {
    ws2811_fini(&ledstring);
}

void LEDStripController::render(ws2811_led_t* array) {
    for (int x = 0; x < LED_COUNT; x++) {
        ledstring.channel[0].leds[x] = array[x];
    }
    ws2811_render(&ledstring);
}

void LEDStripController::clear(ws2811_led_t* array) {
    for (int x = 0; x < LED_COUNT; x++) {
        array[x] = 0;
    }
    render(array);
}
