#include <csignal>

#include <iostream>
#include <thread>
#include <chrono>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "controllers/LEDColorController.h"
#include "controllers/InputSourceController.h"
#include "sources/VideoSource.h"
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


using namespace std::chrono;

//void testSleepDuration(milliseconds sleepDuration) {
//	// Record the start time
//	auto startTime = high_resolution_clock::now();
//
//	// Perform the sleep
//	timeBeginPeriod(1); // Set the system timer resolution to 1 ms
//	std::this_thread::sleep_for(sleepDuration);
//	timeEndPeriod(1); // Set the system timer resolution to 1 ms
//
//	// Record the end time
//	auto endTime = high_resolution_clock::now();
//
//	// Calculate the actual sleep time
//	auto actualSleepTime = duration_cast<milliseconds>(endTime - startTime);
//
//	// Output the results
//	std::cout << "Requested sleep duration: " << sleepDuration.count() << " ms" << std::endl;
//	std::cout << "Actual sleep duration: " << actualSleepTime.count() << " ms" << std::endl;
//}

int main()
{
	// Increase timer resolution to 1 ms


	// Test various sleep durations
	//std::cout << "Capped to 15ms" << std::endl;
	//testSleepDuration(milliseconds(5));  // 10 milliseconds
	//testSleepDuration(milliseconds(8));  // 10 milliseconds
	//testSleepDuration(milliseconds(10));  // 10 milliseconds
	//testSleepDuration(milliseconds(12));  // 10 milliseconds
	//testSleepDuration(milliseconds(14));  // 10 milliseconds

	//std::cout << "Capped to 30ms" << std::endl;
	//testSleepDuration(milliseconds(20));  // 20 milliseconds
	//testSleepDuration(milliseconds(25));  // 20 milliseconds
	//testSleepDuration(milliseconds(28));  // 20 milliseconds
	//testSleepDuration(milliseconds(29));  // 20 milliseconds

	//std::cout << "Capped to 60ms" << std::endl;
	//testSleepDuration(milliseconds(32));  // 20 milliseconds
	//testSleepDuration(milliseconds(35));  // 20 milliseconds
	//testSleepDuration(milliseconds(38));  // 20 milliseconds
	//testSleepDuration(milliseconds(40));  // 20 milliseconds
	//testSleepDuration(milliseconds(45));  // 20 milliseconds
	//testSleepDuration(milliseconds(50));  // 50 milliseconds
	//testSleepDuration(milliseconds(55));  // 50 milliseconds

	//testSleepDuration(milliseconds(100)); // 100 milliseconds
	//testSleepDuration(milliseconds(200)); // 200 milliseconds
	//testSleepDuration(milliseconds(300)); // 200 milliseconds
	//testSleepDuration(milliseconds(400)); // 200 milliseconds
	//testSleepDuration(milliseconds(600)); // 200 milliseconds

	// Reset timer resolution


	std::signal(SIGINT, signalHandler);
	Debug::set(true);

	int numLedWidth = 13;
	int numLedHeight = 6;
	bool showBottom = true;

	// Initialize Color controller
	LEDColorController ledColorController{ numLedWidth, numLedHeight, showBottom };

	// Inititalize Video source
	std::string videoPath = "D:/resources/videos/Ambilight.mp4";
	VideoSource videoSource{ videoPath };


	RenderManager renderManager{ &ledColorController };
	// Assign address of renderManager object to global pointer
	g_pRenderManager = &renderManager;

	renderManager.setFrameSource(&videoSource);
	//renderManager.setResolution(640, 480);
	//renderManager.setFPS(60);
	renderManager.setColorMode(ColorMode::DYNAMIC);

	std::cout << renderManager.toString() << std::endl;
	
	renderManager.render();
	
	std::cout << "Exiting program..." << std::endl;
	return 0;
}