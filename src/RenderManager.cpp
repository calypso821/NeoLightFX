#include "RenderManager.h"

#include <chrono>
#include <thread>

#include "utils/Debug.h"
#include "utils/EnumToString.h"
#include "utils/platform_detection.h"

RenderManager::RenderManager(LEDColorController* colorController)
	: m_running(false),
	m_pLedControlController(colorController),
	m_pFrameSource(nullptr),
	m_pSimController(nullptr)
{
	init();
}

void RenderManager::init()
{
	setRenderMode(RenderMode::SIMULATE);
	setColorMode(ColorMode::STATIC);
	setResolution(1280, 720);
	setFPS(15.0f);
	setStaticColor(0xFF);
	


	std::cout << "Render manager initialization: Success" << std::endl;
	//std::cout << toString() << std::endl;

}

std::string RenderManager::toString() const
{
	std::ostringstream oss;
	oss << "Render mode: " << renderModeToString(m_renderMode) << "\n"
		<< "Color mdoe: " << colorModeToString(m_colorMode) << "\n"
		<< "Target resolution: " << m_resolution.first << "x" << m_resolution.second << "\n"
		<< "Target FPS: " << m_fps;
	return oss.str();
}

/* Functions canot be set during runtime */
void RenderManager::setFPS(float fps)
{
	if (isRunning()) {
		std::cerr << "Cannot set FPS while running" << std::endl;
		return;
	}
	m_fps = fps;
}
void RenderManager::setResolution(int width, int height)
{
	if (isRunning()) {
		std::cerr << "Cannot set resolution while running" << std::endl;
		return;
	}
	m_resolution = std::make_pair(width, height);
}
void RenderManager::setFrameSource(FrameSource* frameSource)
{
	if (isRunning()) {
		// Recoverable condition
		std::cerr << "Cannot set Frame source while running" << std::endl;
		return;
	}

	if (!frameSource) {
		// Unintended use of function -> throw exception
		throw std::invalid_argument("Frame source cannot be null");
	}

	m_pFrameSource = frameSource;
	// Set resolution of frame source
	m_resolution = frameSource->getResolution();
	m_fps = frameSource->getFPS();
}

void RenderManager::setRenderMode(RenderMode renderMode)
{
	if (isRunning()) {
		std::cerr << "Cannot set render mode while running" << std::endl;
		return;
	}

	if (renderMode == RenderMode::HARDWARE)
	{
		std::cerr << "Hardware render mode not supported in this version.\n";
		return; // Exit early as hardware mode is not supported
	}

	if (renderMode == RenderMode::SIMULATE)
	{
		m_pSimController = new SimulationController{ m_pLedControlController };
	}

	m_renderMode = renderMode;
}

/* Functions can be set during runtime */
void RenderManager::setColorMode(ColorMode colorMode)
{
	// Can be set during runtime
	if (colorMode == ColorMode::DYNAMIC)
	{
		// Initialize frame processor
		m_pLedControlController->initFrameProcessor(m_resolution.first, m_resolution.second);
	}
	m_colorMode = colorMode;
}

void RenderManager::setStaticColor(uint32_t color)
{
	// Set static color
	m_pLedControlController->setStaticColor(color);

	// Set static color of frame
	if (m_renderMode == RenderMode::SIMULATE)
	{
		m_frame = m_pSimController->createSolidColorFrame(m_resolution.first, m_resolution.second, color);
		if (m_frame.empty()) {
			std::cerr << "Solid frame initialization failed" << std::endl;
		}
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

/* Control flow functions */
void RenderManager::stop()
{
	m_running.store(false);
}

bool RenderManager::isRunning()
{
	return m_running.load();
}

void RenderManager::render()
{
	// Set sleep timer percision 
	/* WINDOWS */
	if (getPlatform() == Platform::Windows) {
		std::cout << "Windows" << std::endl;
		timeBeginPeriod(1);
	}

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
	std::cout << "Frame duration: " << frameDuration.count() << " ms" << std::endl;

	// Init previousTime
	// auto -> automatically deduce type of variable
	auto previousTime = steady_clock::now();

	uint32_t* pColorArray = m_pLedControlController->getColorArray();
	bool showBottom = m_pLedControlController->getBottomStatus();

	int targetWidth = m_resolution.first;
	int targetHeight = m_resolution.second;

	cv::Size targetSize(targetWidth, targetHeight);
	cv::Mat resizedFrame;

	m_running.store(true);

	
	std::cout << "Rendering started..."<< std::endl;

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
					m_pLedControlController->setColorBySource(m_frame);
				}
			}

			// Resize the frame to target resolution
			cv::resize(m_frame, resizedFrame, targetSize);
			/* Simulate LED strip */
			m_pSimController->simulate(resizedFrame);
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
		duration<float, std::milli> frameTime = frameEndTime - currentTime;
		//std::cout << "Frame time: " << frameTime.count() << " ms" << std::endl;

		if (frameTime < frameDuration) {
			auto sleepTime = frameDuration - frameTime;
			//std::cout << "Sleep time: " << sleepTime.count() << " ms" << std::endl;
			auto sleepTimeMs = duration_cast<std::chrono::milliseconds>(sleepTime);
			std::this_thread::sleep_for(sleepTimeMs);
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
	// Reset sleep timer percision 
	/* WINDOWS */
	if (getPlatform() == Platform::Windows) {
		timeEndPeriod(1);
	}

	std::cout << "Rendering stopped..." << std::endl;

}

