#include <csignal>

#include "controllers/LEDColorController.h"
#include "controllers/InputSourceController.h"
#include "sources/VideoSource.h"
#include "RenderManager.h"

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

	int numLedWidth = 50;
	int numLedHeight = 27;
	bool showBottom = true;

	// Initialize Color controller
	LEDColorController ledColorController{ numLedWidth, numLedHeight, showBottom };

	// Inititalize Video source
	VideoSource videoFileSource;
	std::string videoPath = "D:/resources/videos/Ambilight.mp4";

	// Initialize Source controller
	if (!videoFileSource.initVideoFile(videoPath))
	{
		std::cerr << "Error: Failed to initialize video file: " << videoPath << std::endl;
		return 1; // Exit or handle the error as needed
	}


	RenderManager renderManager{ &ledColorController };
	// Assign address of renderManager object to global pointer
	g_pRenderManager = &renderManager;

	//renderManager.setRenderMode(RenderMode::SIMULATE);
	renderManager.render();

	return 0;
}