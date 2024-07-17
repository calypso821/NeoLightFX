#ifndef LEDSTRIPCONTROLLER_H
#define LEDSTRIPCONTROLLER_H

class LEDStripController {
public:
    virtual ~LEDStripController() {}

    virtual void init() = 0;
    virtual void clear() = 0;
    virtual void render() = 0;
    //virtual void setBrightness(int brightness) = 0;
};

#endif // LEDSTRIPCONTROLLER_H
