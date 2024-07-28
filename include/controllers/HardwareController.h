#ifndef HARDWARECONTROLLER_H
#define HARDWARECONTROLLER_H

#include <cstdint>

#include <opencv2/opencv.hpp>

#include "controllers/VisualController.h"
#include "controllers/LEDColorController.h"
#include "hardware/LEDStripController.h"

class HardwareController : public VisualController
{
public:
    HardwareController(LEDColorController* colorController);
    ~HardwareController() override;

    void initialize(int ledCount) override;
    void shutdown() override;
    void renderFrame() override;
    void clearFrame() override;

private:
    LEDStripController* m_pLedStripController;
};

#endif //HARDWARECONTROLLER_H