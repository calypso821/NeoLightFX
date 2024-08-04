#include <csignal>
#include <thread>

#include <opencv2/core/utils/logger.hpp>

#include "controllers/LEDColorController.h"
#include "hardware/LEDStripConfig.h"
#include "controllers/SimulationController.h"
#include "RenderManager.h"
#include "utils/Debug.h"

// Global pointer to RenderManager
RenderManager* g_pRenderManager = nullptr;
std::atomic<bool> isRunning = true;

static void signalHandler(int signal)
{
	isRunning.store(false);
	if (signal == SIGINT) {
		if (g_pRenderManager) {
			g_pRenderManager->stop();
		}
	}
}

int main()
{
	//std::signal(SIGINT, signalHandler);
	Debug::set(false);
	std::string mode = "static";

	// Set OpenCV logging level to show only errors
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

	LEDStripConfig stripConfig{ 10 };
	LEDColorController colorController{ stripConfig };
	colorController.setStaticColor(0xFF0000);

	uint32_t* p_colorArray = colorController.getColorArray();

	SimulationController simController{ &colorController };
	while (isRunning.load())
	{
		static int counter = 0;
		for (int i = 0; i < stripConfig.count; i++)
		{
			uint8_t r = counter * 20;
			uint8_t g = counter * 1;
			uint8_t b = counter * 50;
			p_colorArray[i] = (r << 16) | (g << 8) | b;
		}
		simController.simulateStrip();
		simController.render();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		
		if (++counter > 20) {
			isRunning.store(false);
		}

	}


	return 0;


	//RenderManager renderManager{ stripConfig };
	//// Assign address of renderManager object to global pointer
	//g_pRenderManager = &renderManager;

	//renderManager.setColorMode(ColorMode::PATTERN);

	//// Set target resolution
	//renderManager.scaleResolution(0.75f);

	//// Print render manager properties
	//std::cout << renderManager.toString() << std::endl;

	//// Start render
	//renderManager.render();

	//return 0;
}