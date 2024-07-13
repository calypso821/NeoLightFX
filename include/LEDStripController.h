#ifndef LEDSTRIPCONTROLLER_H
#define LEDSTRIPCONTROLLER_H

#include <ws2811.h>

class LEDStripController {
public:
    LEDStripController(int brightness);
    ~LEDStripController();
    void render(ws2811_led_t* array);
    void clear(ws2811_led_t* array);

private:
    ws2811_t ledstring;
    ws2811_return_t ret;
};

#endif // LEDSTRIPCONTROLLER_H
