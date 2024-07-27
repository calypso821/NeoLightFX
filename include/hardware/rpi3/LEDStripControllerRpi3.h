#ifndef LEDSTRIPCONTROLLERRPI3_H
#define LEDSTRIPCONTROLLERRPI3_H

#include "hardware/LEDStripController.h"

#include "rpi_281x/ws2811.h"

class LEDStripControllerRpi3 : public LEDStripController {
public:
	// Constructor
	LEDStripControllerRpi3(int ledCount);

	// Destructor
	~LEDStripControllerRpi3() override;

	void init() override;
	void render() override;

	void ledArraySetColor(uint32_t* colorArray) override;
	void ledArrayClear() override;
	void setBrightness(int brightness) override;

private:
	ws2811_t m_ledArray;
};

#endif // LEDSTRIPCONTROLLERRPI3_H

