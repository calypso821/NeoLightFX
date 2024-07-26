#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include <utility>
#include <atomic>

#include <opencv2/core/mat.hpp>

#include "sources/FrameSource.h"
#include "controllers/SimulationController.h"
#include "controllers/LEDColorController.h"

enum class RenderMode
{
	SIMULATE,
	HARDWARE
};

enum class ColorMode
{
	STATIC,
	DYNAMIC,
	//PATTERN,
	//SOUND
};

enum class Color {
	Red = 0xFF0000,
	Green = 0x00FF00,
	Blue = 0x0000FF,
	White = 0xFFFFFF,
	Black = 0x000000
};

class RenderManager
{
public:
	RenderManager(LEDColorController* colorController);

	void setFPS(float fps);
	void setResolution(int width, int height);
	void setRenderMode(RenderMode renderMode);
	void setColorMode(ColorMode colorMode);

	void setFrameSource(FrameSource* frameSource);
	void setColorByName(Color color);
	void setColorByHexCode(uint32_t color);
	
	void render();
	void stop();
	bool isRunning();

	std::string toString() const;
	
	
private:
	std::atomic<bool> m_running;
	RenderMode m_renderMode;
	ColorMode m_colorMode;

	std::pair<int, int> m_resolution;
	float m_fps;

	LEDColorController* m_pLedControlController;
	SimulationController* m_pSimController;
	FrameSource* m_pFrameSource;
	cv::Mat m_frame;

	void init();
	void setStaticColor(uint32_t color);
	
};

#endif // RENDERMANAGER_H