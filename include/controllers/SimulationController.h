#ifndef SIMULATIONCONTROLLER_H
#define SIMULATIONCONTROLLER_H

#include <cstdint>

#include <opencv2/opencv.hpp>

#include "controllers/VisualController.h"
#include "controllers/LEDColorController.h"

class SimulationController : public VisualController
{
public:
	SimulationController(LEDColorController* colorController);

    //void initialize() override;
    void shutdown() override;
    void renderFrame() override;
    void clearFrame() override;


    void simulate(const cv::Mat& frame);
    void resizeFrame(const cv::Mat& frame, cv::Mat& resizedFrame, cv::Size targetSize);
    cv::Mat createSolidColorFrame(int width, int height, uint32_t color);

private:
    cv::Mat m_canvas;
};

#endif //SIMULATIONCONTROLLER_H