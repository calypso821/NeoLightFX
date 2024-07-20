#ifndef INPUTSOURCECONTROLLER_H
#define INPUTSOURCECONTROLLER_H

#include "sources/FrameSource.h"
#include "controllers/LEDColorController.h"

class InputSourceController {
public:
    InputSourceController(FrameSource* frameSource, LEDColorController* ledColorController);
    void start();

private:
    FrameSource* m_pFrameSource;
    LEDColorController* m_pLedColorController;
};

#endif // INPUTSOURCECONTROLLER_H
