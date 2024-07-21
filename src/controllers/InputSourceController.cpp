#include "controllers/InputSourceController.h"
#include <iostream>
#include <chrono>

InputSourceController::InputSourceController(FrameSource* frameSource, LEDColorController* ledColorController)
    : m_pFrameSource(frameSource), m_pLedColorController(ledColorController) {}

void InputSourceController::init()
{
    std::pair<int, int> res = m_pFrameSource->getResolution();
    m_pLedColorController->initImageProcessor(res.first, res.second);
}

void InputSourceController::start()
{
    cv::Mat frame;
    // TODO: FrameRate

    bool showFrame = true;

    // using -> type alias
    using clock = std::chrono::steady_clock;
    // auto -> automatically deduce type of variable
    auto previousTime = clock::now();

    while (m_pFrameSource->getNextFrame(frame))
    {
        auto currentTime = clock::now();
        // std::chrono::duration object -> stores the duration in seconds
        std::chrono::duration<double> elapsedSeconds = currentTime - previousTime;
        previousTime = currentTime;

        // Calculate FPS
        // count() -> used to get the duration value in seconds (as a double)
        double fps = 1.0 / elapsedSeconds.count();
        std::cout << "Time elapsed: " << elapsedSeconds.count() << " s" << std::endl;
        std::cout << "Current frame rate: " << fps << " FPS" << std::endl;

        m_pLedColorController->setColorBySource(frame);

        if (showFrame)
        {
            // Display the frame
            cv::imshow("Frame", frame);

            // Check for user input to exit loop
            if (cv::waitKey(1) == 27) { // Wait for 1 ms and check if 'ESC' key is pressed
                break;
            }
        }
    }

    // Destroy the window
    cv::destroyWindow("Frame");
}
