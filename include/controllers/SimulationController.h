#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <cstdint>

#include <opencv2/opencv.hpp>

#include "controllers/LEDColorController.h"


class SimulationController
{


public:
	SimulationController(LEDColorController* colorController);
    void simulate(const cv::Mat& frame);
    cv::Mat createSolidColorFrame(int width, int height, uint32_t color);


private: 
    LEDColorController* m_pLedColorController;

};

#endif //SIMULATIONCONTROLLER_H