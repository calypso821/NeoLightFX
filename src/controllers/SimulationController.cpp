#include "controllers/SimulationController.h"

SimulationController::SimulationController(LEDColorController* colorController)
	: m_pLedColorController(colorController)
{
}

void SimulationController::simulate(const cv::Mat& frame) {
    assert(!frame.empty() && "Frame is empty");
    try {
        if (frame.empty()) {
            throw std::runtime_error("Input frame is empty");
        }

        std::pair<int, int> ledRes = m_pLedColorController->getLedResolution();
        int ledNum_width = ledRes.first;
        int ledNum_height = ledRes.second;
        int showBottom = m_pLedColorController->getBottomStatus();
        uint32_t* colorArray = m_pLedColorController->getColorArray();

        int width = frame.cols;
        int height = frame.rows;

        int verticalLedSize = height / ledNum_height;
        int horizontalLedSize = width / ledNum_width;

        int borderThickness = 2; // Thickness of the border
        cv::Scalar borderColor = cv::Scalar(0, 0, 0); // White border

        // Add border to the frame
        cv::Mat frameWithBorder;
        cv::copyMakeBorder(
            frame,
            frameWithBorder,
            borderThickness,
            borderThickness,
            borderThickness,
            borderThickness,
            cv::BORDER_CONSTANT,
            borderColor
        );

        // Check if frameWithBorder is valid
        if (frameWithBorder.empty()) {
            throw std::runtime_error("frameWithBorder is empty after adding border");
        }

        // Create a larger canvas
        int canvasWidth = frame.cols + verticalLedSize * 2 + borderThickness * 2;
        int canvasHeight = frame.rows + horizontalLedSize * 2 + borderThickness * 2;
        cv::Mat canvas(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(0, 0, 0));

        // Check if canvas is valid
        if (canvas.empty()) {
            throw std::runtime_error("Canvas is empty after initialization");
        }

        // Copy the frame with border to the center of the canvas
        frameWithBorder.copyTo(canvas(cv::Rect(verticalLedSize, horizontalLedSize, frameWithBorder.cols, frameWithBorder.rows)));

        // Draw Top, Bottom
        int offset_t = ledNum_height;
        int offset_b = ledNum_height * 2 + ledNum_width;
        for (int i = 0; i < ledNum_width; ++i) {
            // Top side
            cv::rectangle(
                canvas,
                // Area
                cv::Rect(
                    i * horizontalLedSize + verticalLedSize + borderThickness,
                    0,
                    horizontalLedSize,
                    horizontalLedSize
                ),
                // Color
                cv::Scalar(
                    (colorArray[i + offset_t] >> 16) & 0xFF,
                    (colorArray[i + offset_t] >> 8) & 0xFF,
                    colorArray[i + offset_t] & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                canvas,
                cv::Point(
                    i * horizontalLedSize + verticalLedSize + borderThickness,
                    0
                ),
                cv::Point(
                    i * horizontalLedSize + verticalLedSize + borderThickness,
                    horizontalLedSize
                ),
                cv::Scalar(0, 0, 0),
                borderThickness
            );

            // Bottom side
            if (showBottom) {
                int i_inverse = ledNum_width - 1 - i;
                cv::rectangle(
                    canvas,
                    // Area
                    cv::Rect(
                        i_inverse * horizontalLedSize + verticalLedSize + borderThickness,
                        canvasHeight - horizontalLedSize,
                        horizontalLedSize,
                        horizontalLedSize
                    ),
                    // Color
                    cv::Scalar(
                        (colorArray[i + offset_b] >> 16) & 0xFF,
                        (colorArray[i + offset_b] >> 8) & 0xFF,
                        colorArray[i + offset_b] & 0xFF
                    ),
                    cv::FILLED
                );
                // Border
                cv::line(
                    canvas,
                    cv::Point(
                        i_inverse * horizontalLedSize + horizontalLedSize + verticalLedSize + borderThickness,
                        canvasHeight - horizontalLedSize
                    ),
                    cv::Point(
                        i_inverse * horizontalLedSize + horizontalLedSize + verticalLedSize + borderThickness,
                        canvasHeight
                    ),
                    cv::Scalar(0, 0, 0),
                    borderThickness
                );
            }
        }

        // Draw Left, Right
        int offset_l = 0;
        int offset_r = ledNum_height + ledNum_width;
        for (int i = 0; i < ledNum_height; ++i) {
            // Left side
            int i_inverse = ledNum_height - 1 - i;
            cv::rectangle(
                canvas,
                // Area
                cv::Rect(
                    0,
                    i_inverse * verticalLedSize + horizontalLedSize + borderThickness,
                    verticalLedSize,
                    verticalLedSize
                ),
                // Color
                cv::Scalar(
                    (colorArray[i + offset_l] >> 16) & 0xFF,
                    (colorArray[i + offset_l] >> 8) & 0xFF,
                    colorArray[i + offset_l] & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                canvas,
                cv::Point(
                    0,
                    i_inverse * verticalLedSize + verticalLedSize + horizontalLedSize + borderThickness
                ),
                cv::Point(
                    verticalLedSize,
                    i_inverse * verticalLedSize + verticalLedSize + horizontalLedSize + borderThickness
                ),
                cv::Scalar(0, 0, 0),
                borderThickness
            );

            // Right side
            cv::rectangle(
                canvas,
                // Area
                cv::Rect(
                    canvasWidth - verticalLedSize,
                    i * verticalLedSize + horizontalLedSize + borderThickness,
                    verticalLedSize,
                    verticalLedSize
                ),
                // Color
                cv::Scalar(
                    (colorArray[i + offset_r] >> 16) & 0xFF,
                    (colorArray[i + offset_r] >> 8) & 0xFF,
                    colorArray[i + offset_r] & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                canvas,
                cv::Point(
                    canvasWidth - verticalLedSize,
                    i * verticalLedSize + horizontalLedSize + borderThickness
                ),
                cv::Point(
                    canvasWidth,
                    i * verticalLedSize + horizontalLedSize + borderThickness
                ),
                cv::Scalar(0, 0, 0),
                borderThickness
            );
        }

        // Show the canvas with LEDs
        cv::imshow("LED Simulation", canvas);
        cv::waitKey(1); // Wait for a short period to update the display
    }
    catch (const cv::Exception& e) {
        std::cerr << "OpenCV exception: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown exception occurred!" << std::endl;
    }
}

cv::Mat SimulationController::createSolidColorFrame(int width, int height, uint32_t color) {
    cv::Mat frame(height, width, CV_8UC3, cv::Scalar((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF));
    return frame;
}
