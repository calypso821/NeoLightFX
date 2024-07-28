#ifndef VISUALCONTROLLER_H
#define VISUALCONTROLLER_H

#include <opencv2/opencv.hpp>

class VisualController {
public:
	virtual ~VisualController() = default;

	// Initialization and Cleanup
	//virtual void initialize() = 0;
	//virtual void shutdown() = 0;

	// Frame Handling
	virtual renderFrame(const cv::Mat& frame) = 0;
	virtual void clearFrame() = 0;
};