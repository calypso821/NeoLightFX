#ifndef LEDSTRIPCONTROLLERRPI3_H
#define LEDSTRIPCONTROLLERRPI3_H

#include "LEDStripController.h"

class LEDStripControllerRpi3 : public LEDStripController {
public:
	void init() override;
	void clear() override;
	void render() override;
	//void setBrightness(int brightness) override;
	//void setColor(uint32_t color) override;
};

#endif // LEDSTRIPCONTROLLERRPI3_H