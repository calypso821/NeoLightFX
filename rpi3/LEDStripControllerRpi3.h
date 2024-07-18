#ifndef LEDSTRIPCONTROLLERRPI3_H
#define LEDSTRIPCONTROLLERRPI3_H

#include "LEDStripController.h"
#include "ws2811.h"

class LEDStripControllerRpi3 : public LEDStripController {

public:
	// Constructor
	LEDStripControllerRpi3(int ledCount);
	// Destructor
	~LEDStripControllerRpi3();
	
	void init() override;
	void render() override;

	// TODO: override
	void led_array_setColor(uint32_t* colorArray);
	void led_array_clear();
	void setBrightness(int brightness);
	
	void clear() override; // TODO: Delete
	
private:
	ws2811_t led_array;
	int led_count;
};

#endif // LEDSTRIPCONTROLLERRPI3_H
