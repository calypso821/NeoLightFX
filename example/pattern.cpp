#include <csignal>
#include <thread>

#include <opencv2/core/utils/logger.hpp>

#include "controllers/LEDColorController.h"
#include "hardware/LEDStripConfig.h"
#include "controllers/SimulationController.h"
#include "RenderManager.h"
#include "utils/Debug.h"
#include "utils/color_processing_utils.h"

uint32_t rainbowColors[] = {
	0xFFA500, // Orange
	0xFFFF00, // Yellow
	0x00FF00, // Green
	0x0000FF, // Blue
	0x4B0082, // Indigo
	0xEE82EE,  // Violet
	0xFF0000, // Red
};

// Optionally, you can define the size of the array
constexpr size_t rainbowColorsSize = sizeof(rainbowColors) / sizeof(rainbowColors[0]);

// Global pointer to RenderManager
RenderManager* g_pRenderManager = nullptr;
std::atomic<bool> isRunning = true;

LEDStripConfig stripConfig{ 21 };

static void signalHandler(int signal)
{
	isRunning.store(false);
	if (signal == SIGINT) {
		if (g_pRenderManager) {
			g_pRenderManager->stop();
		}
	}
}



void rainbowEffect(uint32_t* p_colorArray, float progress)
{
	for (size_t i = 0; i < stripConfig.count; ++i) {
		float ledPosition = static_cast<float>(i) / stripConfig.count;
		float colorPosition = fmod(progress + ledPosition, 1.0f) * rainbowColorsSize;

		size_t colorIndex1 = static_cast<size_t>(colorPosition);
		size_t colorIndex2 = (colorIndex1 + 1) % rainbowColorsSize;

		float t = colorPosition - colorIndex1;

		p_colorArray[i] = lerpColor(rainbowColors[colorIndex1], rainbowColors[colorIndex2], t);
	}
}

int main()
{
	//std::signal(SIGINT, signalHandler);
	Debug::set(false);
	std::string mode = "static";

	// Set OpenCV logging level to show only errors
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

	LEDColorController colorController{ stripConfig };
	colorController.setStaticColor(0xFF0000);

	uint32_t* p_colorArray = colorController.getColorArray();
	SimulationController simController{ &colorController };


	static float progress = 0;
	static int prog = 0;
	static int lengthProg = stripConfig.count / 2;

	while (isRunning.load())
	{
		if (progress > 1.0f)
		{
			// Reset progress (start again)
			progress = 0;
		}

		//rainbowEffect(p_colorArray, progress);

		if (prog <= lengthProg)
		{
			for (size_t i = 0; i <= lengthProg; i++)
			{
				p_colorArray[i] = 0;
				p_colorArray[stripConfig.count - 1 - i] = 0;

				p_colorArray[prog] = 0xFFFFFF;
				p_colorArray[stripConfig.count - 1 - prog] = 0xFFFFFF;
			}
			prog++;
		}
		else
		{
			lengthProg--;
			prog = 0;
		}

		

		simController.simulateStrip();
		simController.render();


		progress += 0.1f;

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
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