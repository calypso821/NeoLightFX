#include "RenderManager.h"

#include <thread>

#include "utils/Debug.h"
#include "utils/mode_utils.h"
#include "utils/timer_utils.h"

RenderManager::RenderManager(const LEDStripConfig& stripConfig)
	: m_running(false),
	m_stripConfig(stripConfig),
	m_pLedColorController(new LEDColorController(stripConfig)),
	m_pFrameSource(nullptr),
	m_pFrameProcessor(nullptr),
	m_pVisualController(nullptr),
	m_pSimController(nullptr)
{
	setRenderMode(RenderMode::SIMULATE);
	setColorMode(ColorMode::STATIC);
	setResolution(1280, 720);
	// Set FPS + frame duration
	setFPS(10.0f);
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
void RenderManager::setResolution(int width, int height)
{
	if (isRunning()) {
		std::cerr << "Cannot set resolution while running" << std::endl;
		return;
	}
	m_resolution = std::make_pair(width, height);
}
void RenderManager::scaleResolution(float scalar)
{
	if (isRunning()) {
		std::cerr << "Cannot set resolution while running" << std::endl;
		return;
	}
	int new_width = static_cast<int>(m_resolution.first * scalar);
	int new_height = static_cast<int>(m_resolution.second * scalar);
	m_resolution = std::make_pair(new_width, new_height);
}

void RenderManager::checkTargetProperties()
{
	if (m_pFrameSource)
	{
		/* RESOLUTION CHECK */
		std::pair<int,int> max_res = m_pFrameSource->getResolution();
		if (max_res.first < m_resolution.first || max_res.second < m_resolution.second)
		{
			setResolution(max_res.first, max_res.second);

			std::cout << "Warning: Target resolution exceeds source resolution "
					  << "(adjusted to " << m_resolution.first << "x" << m_resolution.second << ")"
				      << std::endl;
		}
		// Initialize FrameProcessor with width, height
		//m_pFrameProcessor = new FrameProcessor();
		// TODO: Mybe add this to Dynamic mode? 
		// And release at STATIC MODDE? 
		m_pFrameProcessor = new FrameProcessor(m_stripConfig, m_resolution.first, m_resolution.second);
		//m_pLedColorController->initializeFrameProcessor(m_resolution.first, m_resolution.second);

		/* FPS CHECK */
		float max_fps = m_pFrameSource->getFPS();
		if (max_fps < m_fps)
		{
			setFPS(max_fps);

			std::cout << "Warning: Target FPS exceeds source FPS "
				<< "(adjusted to " << m_fps << ")" << std::endl;
		}
	}	
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

	auto res = frameSource->getResolution();
	setResolution(res.first, res.second);
	setFPS(frameSource->getFPS());
}

void RenderManager::setRenderMode(RenderMode renderMode)
{
	if (isRunning()) {
		std::cerr << "Cannot set render mode while running" << std::endl;
		return;
	}

	if (renderMode == RenderMode::SIMULATE)
	{
		m_pVisualController = new SimulationController{ m_pLedColorController };
	}
	#ifdef HARDWARE_MODE
	if (renderMode == RenderMode::HARDWARE)
	{
		m_pVisualController = new HardwareController{ m_pLedColorController };
	}
	#endif

	m_renderMode = renderMode;
}

/* Functions can be set during runtime */
void RenderManager::setFPS(float fps)
{
	// The count() method returns the number of milliseconds as a float,
	// Calculate the frame duration in milliseconds with floating-point precision
	m_frameDuration = std::chrono::duration<float, std::milli>(1000.0f / fps);
	m_fps = fps;
	
}
void RenderManager::setColorMode(ColorMode colorMode)
{
	if (colorMode == ColorMode::STATIC)
	{
		setFPS(10.0f);
	}
	if (colorMode == ColorMode::DYNAMIC)
	{
		if (m_pFrameSource) {
			setFPS(m_pFrameSource->getFPS());
		}
	}
	m_colorMode = colorMode;
}

void RenderManager::setStaticColor(uint32_t color)
{
	// Set static color
	m_pLedColorController->setStaticColor(color);

	if (m_pVisualController)
	{
		// Set static color of frame
		if (m_renderMode == RenderMode::SIMULATE)
		{
			m_pSimController = dynamic_cast<SimulationController*>(m_pVisualController);
			m_frame = m_pSimController->createSolidColorFrame(m_resolution.first, m_resolution.second, color);
			if (m_frame.empty()) {
				std::cerr << "Solid frame initialization failed" << std::endl;
			}
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
	const int GREYSCALE_CORRECTION = true;
	// Set sleep timer percision 
	setPercisionTimer();

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

	// Init previousTime
	// auto -> automatically deduce type of variable
	auto previousTime = steady_clock::now();

	uint32_t* pColorArray = m_pLedColorController->getColorArray();

	// Check and adjust target properties (+ init FrameProcessor)
	checkTargetProperties();

	int targetWidth = m_resolution.first;
	int targetHeight = m_resolution.second;

	cv::Size targetSize(targetWidth, targetHeight);
	cv::Mat resizedFrame;

	// Visual controller initialization
	m_pVisualController->initialize();

	m_running.store(true);
	bool hasNextFrame;

	
	std::cout << "Rendering started..."<< std::endl;

	while (isRunning())
	{
		auto currentTime = steady_clock::now();
		// std::chrono::duration object -> stores the duration in seconds
		duration<double> elapsed = currentTime - previousTime;
		previousTime = currentTime;

		/* USER CODE - START */

		/* DYNAMIC MODE */
		if (m_colorMode == ColorMode::DYNAMIC)
		{
			if (m_pFrameSource)
			{
				// Get next frame from frame source
				hasNextFrame = m_pFrameSource->getNextFrame(m_frame);
				if (!hasNextFrame) {
					break;
				}
				m_pFrameProcessor->processFrame(pColorArray, m_frame, elapsed.count());
				//m_pLedColorController->setColorBySource(m_frame);
			}
		}

		/* SIMULATION MODE */
		if (m_renderMode == RenderMode::SIMULATE)
		{
			// Resize the frame to target resolution
			m_pSimController->resizeFrame(m_frame, resizedFrame, targetSize);
			/* Simulate LED strip */
			m_pSimController->simulateFrame(resizedFrame);
		}

		/* RENDER (display/led strip) */
		m_pVisualController->render();
		
		/* USER CODE - END */

		// count() -> used to get the duration value in seconds (as a double)
		if (Debug::status()) 
		{
			//std::cout << "Time elapsed: " << elapsed.count() << " s" << std::endl;
			std::cout << "Current frame rate: " << 1.0 / elapsed.count() << " FPS" << std::endl;
		}
		
		// Sleep for the remaining time of the frame
		auto frameEndTime = steady_clock::now();
		duration<float, std::milli> frameTime = frameEndTime - currentTime;
		//std::cout << "Frame time: " << frameTime.count() << " ms" << std::endl;

		if (frameTime < m_frameDuration) {
			auto sleepTime = m_frameDuration - frameTime;
			//std::cout << "Sleep time: " << sleepTime.count() << " ms" << std::endl;
			auto sleepTimeMs = duration_cast<std::chrono::milliseconds>(sleepTime);
			std::this_thread::sleep_for(sleepTimeMs);
		} else {
			if (Debug::status()) {
				std::cerr << "Warning: Frame rendering took longer than the target frame duration.\n";
			}
		}
	}

	// Visual controller shutdown (clear windows...)
	m_pVisualController->shutdown();

	// Reset sleep timer percision 
	resetPercisionTimer();
	std::cout << "Rendering stopped..." << std::endl;

}
