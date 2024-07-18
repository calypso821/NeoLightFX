#ifndef LEDSTRIPCONTROLLER_H
#define LEDSTRIPCONTROLLER_H

class LEDStripController {
public:
    // Constructor
    LEDStripController(int ledCount) : m_ledCount(ledCount) {}

    // Virtual Destructor
    virtual ~LEDStripController() {}

    // Pure virtual functions
    virtual void init() = 0;
    virtual void render() = 0;

    virtual void led_array_setColor(uint32_t* colorArray) = 0;
    virtual void led_array_clear() = 0;
    virtual void setBrightness(int brightness) = 0;

protected:
    int m_ledCount; // Number of LEDs
};

#endif // LEDSTRIPCONTROLLER_H
