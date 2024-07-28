#ifndef VISUALCONTROLLER_H
#define VISUALCONTROLLER_H

#include "controllers/LEDColorController.h"

class VisualController
{
public:
	VisualController(LEDColorController* colorController)
		: m_pLedColorController(colorController),
		m_pColorArray(colorController->getColorArray())
	{};
	virtual ~VisualController() = default;

	// Initialization and Cleanup
	virtual void initialize() {};
	virtual void shutdown() {};

	// Frame Handling
	virtual void renderFrame() = 0;
	virtual void clearFrame() = 0;

protected: 
	LEDColorController* m_pLedColorController;
	uint32_t* m_pColorArray;
};

#endif //VISUALCONTROLLER_H