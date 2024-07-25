#include "RenderManager.h"

#include <chrono>
#include <thread>

#include "utils/SimulationUtils.h"
#include "Debug.h"


RenderManager::RenderManager(LEDColorController* colorController)
	: m_running(true),
	m_renderMode(RenderMode::SIMULATE),
	m_colorMode(ColorMode::STATIC),
	m_pLedControlController(colorController),
	m_pFrameSource(nullptr),
	m_resolution(1280, 720),
	m_fps(15.0f)
{
	setStaticColor(0x000000);
}

void RenderManager::setFPS(float fps)
{
	m_fps = fps;
}
void RenderManager::setResolution(int width, int height)
{
	m_resolution = std::make_pair(width, height);
}
void RenderManager::setRenderMode(RenderMode renderMode)
{
	m_renderMode = renderMode;
}
void RenderManager::setColorMode(ColorMode colorMode)
{
	if (colorMode == ColorMode::DYNAMIC)
	{
		// Initialize frame processor
		m_pLedControlController->initFrameProcessor(m_resolution.first, m_resolution.second);
	}
	m_colorMode = colorMode;
}
void RenderManager::setFrameSource(FrameSource* frameSource)
{
	m_pFrameSource = frameSource;
	// Set resolution of frame source
	m_resolution = frameSource->getResolution();
	m_fps = frameSource->getFPS();
}
void RenderManager::setStaticColor(uint32_t color)
{
	// Set static color
	m_pLedControlController->setStaticColor(color);

	// Set static color of frame
	if (m_renderMode == RenderMode::SIMULATE)
	{
		m_frame = createSolidColorFrame(m_resolution.first, m_resolution.second, color);
	}
	if (m_renderMode == RenderMode::HARDWARE)
	{
		// Check if folder hardware exists 
		// If not throw error hardware support not implemented in this version
	}
}
void RenderManager::setColorByName(Color color)
{
	setStaticColor(static_cast<uint32_t>(color));
}
void RenderManager::setColorByHexCode(uint32_t color)
{
	setStaticColor(color);
}

void RenderManager::stop()
{
	m_running = false;
}

bool RenderManager::isRunning()
{
	return m_running.load();
}

void RenderManager::render()
{
	using namespace std::chrono;

	
	// std::chrono::duration<Rep, Period> 
	// Rep: The representation type, which is the type used to store the number of ticks.
	// Period: The tick period, which represents the ratio of one tick to one second
	// std::chrono::milliseconds - Represents a duration in milliseconds.
	
	//std::chrono::duration<float, std::milli>
	// 1. float (Rep): The representation type is float. 
	// This means the duration can have fractional values, 
	// which allows for more precision compared to an integral type.
	// 2. std::milli (Period): The period is std::milli, which represents milliseconds.
	// std::milli is a typedef for std::ratio<1, 1000>, meaning one millisecond is 1/1000 of a second.
	// Used for precise timing in milliseconds

	// The count() method returns the number of milliseconds as a float,
	// Calculate the frame duration in milliseconds with floating-point precision
	auto frameDuration = duration<float, std::milli>(1000.0f / m_fps);

	// Init previousTime
	// auto -> automatically deduce type of variable
	auto previousTime = steady_clock::now();

	uint32_t* pColorArray = m_pLedControlController->getColorArray();
	bool showBottom = m_pLedControlController->getBottomStatus();

	int targetWidth = m_resolution.first;
	int targetHeight = m_resolution.second;

	cv::Size targetSize(targetWidth, targetHeight);
	cv::Mat resizedFrame;

	while (isRunning())
	{
		auto currentTime = steady_clock::now();
		// std::chrono::duration object -> stores the duration in seconds
		duration<double> elapsed = currentTime - previousTime;
		previousTime = currentTime;

		/* USER CODE - START */

		if (m_renderMode == RenderMode::SIMULATE)
		{
			/* DYNAMIC MODE */
			if (m_colorMode == ColorMode::DYNAMIC)
			{
				if (m_pFrameSource)
				{
					// Get next frame from frame source
					m_pFrameSource->getNextFrame(m_frame);
					// Resize the frame to target resolution
					cv::resize(m_frame, resizedFrame, targetSize);
					m_pLedControlController->setColorBySource(resizedFrame);
				}
			}

			/* Simulate LED strip */
			simulateLEDs(resizedFrame, pColorArray, targetWidth, targetHeight, showBottom);
		}

		/* USER CODE - END */

		// count() -> used to get the duration value in seconds (as a double)
		if (Debug::status()) 
		{
			std::cout << "Time elapsed: " << elapsed.count() << " s" << std::endl;
			std::cout << "Current frame rate: " << 1.0 / elapsed.count() << " FPS" << std::endl;
		}
		
		// Sleep for the remaining time of the frame
		auto frameEndTime = steady_clock::now();

		// Get time elapsed for processing and rendering the current frame
		duration<float, std::milli> frameTime = frameEndTime - currentTime;

		if (frameTime < frameDuration) {
			std::this_thread::sleep_for(duration_cast<milliseconds>(frameDuration - frameTime));
		} else {
			if (Debug::status()) {
				std::cerr << "Warning: Frame rendering took longer than the target frame duration.\n";
			}
		}
	}

	if (m_renderMode == RenderMode::SIMULATE)
	{
		cv::destroyAllWindows();
	}

}

