#ifndef VISUALCONTROLLER_H
#define VISUALCONTROLLER_H

#include "controllers/LEDColorController.h"
#include "hardware/LEDStripConfig.h"

class VisualController
{
public:
	VisualController(LEDColorController* colorController)
		: m_pLedColorController(colorController),
		m_pColorArray(colorController->getColorArray()),
		m_stripConfig(colorController->getStripConfig())
	{};
	virtual ~VisualController() = default;

	// Initialization and Cleanup
	virtual void initialize() {};
	virtual void shutdown() {};

	// Frame Handling
	virtual void render() = 0;
	virtual void clear() = 0;

protected: 
	const LEDStripConfig& m_stripConfig;
	LEDColorController* m_pLedColorController;
	uint32_t* m_pColorArray;
};

#endif //VISUALCONTROLLER_H