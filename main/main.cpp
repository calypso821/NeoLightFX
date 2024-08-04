#include <csignal>

#include <opencv2/core/utils/logger.hpp>

#include "controllers/LEDColorController.h"
#include "hardware/LEDStripConfig.h"
#include "sources/VideoSource.h"
#include "sources/CaptureSource.h"
#include "sources/ImageSource.h"
#include "RenderManager.h"
#include "utils/Debug.h"

// Global pointer to RenderManager
RenderManager* g_pRenderManager = nullptr;

static void signalHandler(int signal)
{
	if (signal == SIGINT) {
		if (g_pRenderManager) {
			g_pRenderManager->stop();
		}
	}
}

int main()
{	
	try 
	{
		std::signal(SIGINT, signalHandler);
		Debug::set(false);
		std::string mode = "static";

		// Set OpenCV logging level to show only errors
		cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

		LEDStripConfig stripConfig{ 34, 20, true };
		
		// File paths
		std::string videoPath = "D:/resources/videos/Ambilight.mp4";
		std::string videoPath1 = "D:/resources/videos/FluidSimHueTest.mp4";
		std::string videoPath2 = "D:/resources/videos/Diablo4Trailer.mp4";
		std::string imagePath = "D:/resources/images/test.jpg";

		RenderManager renderManager{ stripConfig };
		// Assign address of renderManager object to global pointer
		g_pRenderManager = &renderManager;

		if (mode == "static")
		{
			/* STATIC COLOR */
			renderManager.setColorMode(ColorMode::STATIC);
			renderManager.setColorByName(Color::Green);
		} 
		else if(mode == "video")
		{
			/* VIDEO SORUCE */
			VideoSource videoSource{ videoPath2 };

			renderManager.setFrameSource(&videoSource);
			//renderManager.setResolution(640, 480);
			//renderManager.setFPS(10);
			renderManager.setColorMode(ColorMode::DYNAMIC);
		}
		else if (mode == "capture")
		{
			/* CAPTURE SORUCE */
			CaptureSource capSource{ 0 };
			//CaptureSource capSource{ 0, 1280, 720, 60 };

			renderManager.setFrameSource(&capSource);
			//renderManager.setFPS(15);
			renderManager.setColorMode(ColorMode::DYNAMIC);
		}
		else if (mode == "image")
		{

			/* IMAGE SOURCE */
			ImageSource imgSource{ imagePath };
			renderManager.setFrameSource(&imgSource);
			//renderManager.setFPS(15);
			renderManager.setColorMode(ColorMode::DYNAMIC);
		}
		else
		{
			throw std::invalid_argument("Unknown mode: " + mode);
		}

		// Set target resolution
		renderManager.scaleResolution(0.75f);

		// Print render manager properties
		std::cout << renderManager.toString() << std::endl;

		// Start render
		renderManager.render();

		std::cout << "Exiting program..." << std::endl;

	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Caught runtime error: " << e.what() << std::endl;
		return 1;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Caught exception: " << e.what() << std::endl;
		return 1;
	}
	
	return 0;
}