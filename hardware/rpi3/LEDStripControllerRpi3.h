#ifndef LEDSTRIPCONTROLLERRPI3_H
#define LEDSTRIPCONTROLLERRPI3_H

#include "LEDStripController.h"

#include "ws2811.h"

class LEDStripControllerRpi3 : public LEDStripController {
public:
	// Constructor
	LEDStripControllerRpi3(int ledCount);

	// Destructor
	~LEDStripControllerRpi3() override;

	void init() override;
	void render() override;

	void led_array_setColor(uint32_t* colorArray) override;
	void led_array_clear() override;
	void setBrightness(int brightness) override;

private:
	ws2811_t led_array;
};

#endif // LEDSTRIPCONTROLLERRPI3_H

