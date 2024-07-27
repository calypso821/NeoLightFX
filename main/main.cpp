#include <csignal>

#include "controllers/LEDColorController.h"
#include "controllers/InputSourceController.h"
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
	std::signal(SIGINT, signalHandler);
	Debug::set(true);

	int numLedWidth = 34;
	int numLedHeight = 21;
	bool showBottom = true;

	// Initialize Color controller
	LEDColorController ledColorController{ numLedWidth, numLedHeight, showBottom };

	// File paths
	std::string videoPath = "D:/resources/videos/Ambilight.mp4";
	std::string videoPath1 = "D:/resources/videos/OW_Widow.mp4";
	std::string videoPath2 = "D:/resources/videos/test2.mp4";
	std::string videoPath3 = "/home/matic/Videos/SampleVideo.mp4";
	std::string imagePath = "D:/resources/images/test.jpg";

	// Initialize source
	VideoSource videoSource{ videoPath3 };
	//CaptureSource capSource{ 0 };
	//CaptureSource capSource{ 0, 1280, 720, 30 };
	//ImageSource imgSource{ imagePath };


	RenderManager renderManager{ &ledColorController };
	// Assign address of renderManager object to global pointer
	g_pRenderManager = &renderManager;

	/* VIDEO SORUCE */
	renderManager.setFrameSource(&videoSource);
	//renderManager.setResolution(640, 480);
	renderManager.setFPS(10);
	renderManager.setColorMode(ColorMode::DYNAMIC);
	std::cout << renderManager.toString() << std::endl;

	/* IMAGE SOURCE */
	/*renderManager.setFrameSource(&imgSource);
	renderManager.scaleResolution(1.5f);
	renderManager.setFPS(15);
	renderManager.setColorMode(ColorMode::DYNAMIC);*/
	//std::cout << renderManager.toString() << std::endl;

	/* CAPTURE SORUCE */
	// renderManager.setFrameSource(&capSource);
	// //renderManager.setFPS(15);
	// renderManager.setColorMode(ColorMode::DYNAMIC);
	// std::cout << renderManager.toString() << std::endl;
	
	renderManager.render();
	
	std::cout << "Exiting program..." << std::endl;
	return 0;
}