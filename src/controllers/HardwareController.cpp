#include "controllers/HardwareController.h"

#include "hardware/rpi3/LEDStripControllerRpi3.h"

HardwareController::HardwareController(LEDColorController* colorController)
	: VisualController(colorController),
	m_pLedStripController(nullptr)
{
}

HardwareController::~HardwareController()
{
	delete m_pLedStripController;
}

void HardwareController::initialize()
{
	int ledCount = m_pLedColorController->getLedCount();
	m_pLedStripController = new LEDStripControllerRpi3{ ledCount };
	m_pLedStripController->initialize();
	m_pLedStripController->ledArrayClear();
}

void HardwareController::shutdown()
{
	m_pLedStripController->ledArrayClear();
	m_pLedStripController->render();
}

void HardwareController::renderFrame()
{
	m_pLedStripController->ledArraySetColor(m_pColorArray);
	m_pLedStripController->render();
}

void HardwareController::clearFrame()
{
	m_pLedStripController->ledArrayClear();
}
