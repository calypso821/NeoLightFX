#include "controllers/SimulationController.h"

SimulationController::SimulationController(LEDColorController* colorController)
	: VisualController(colorController)
{
}

void SimulationController::shutdown()
{
    cv::destroyAllWindows();
}

void SimulationController::render()
{
    // Show the canvas with LEDs
    cv::imshow("LED Simulation", m_canvas);
    cv::waitKey(1); // Wait for a short period to update the display
}
void SimulationController::clear()
{
    // Clear m_canvas by setting all its pixels to black
    m_canvas.setTo(cv::Scalar(0, 0, 0));
}

void SimulationController::simulateStrip()
{
    // To fit 75% of screen? 
    // Start filling next line??
    int ledSize = 30;
    m_canvas = cv::Mat(ledSize, ledSize * m_stripConfig.count, CV_8UC3, cv::Scalar(0, 0, 0));

    int borderThickness = 2; // Thickness of the border

    for (int i = 0; i < m_stripConfig.count; ++i) {
        // Top side
        cv::rectangle(
            m_canvas,
            // Area
            cv::Rect(
                i * ledSize, // + borderThickness,
                0,
                ledSize,
                ledSize
            ),
            // Color
            cv::Scalar(
                m_pColorArray[i] & 0xFF,
                (m_pColorArray[i] >> 8) & 0xFF,
                (m_pColorArray[i] >> 16) & 0xFF
            ),
            cv::FILLED
        );

        // Border
        cv::line(
            m_canvas,
            cv::Point(
                i * ledSize - borderThickness,
                0
            ),
            cv::Point(
                i * ledSize - borderThickness,
                ledSize
            ),
            cv::Scalar(0, 0, 0),
            borderThickness
        );
    }

}

void SimulationController::simulateFrame(const cv::Mat& frame) {
    assert(!frame.empty() && "Frame is empty");
    try {
        if (frame.empty()) {
            throw std::runtime_error("Input frame is empty");
        }

        int width = frame.cols;
        int height = frame.rows;

        int verticalLedSize = height / m_stripConfig.height;
        int horizontalLedSize = width / m_stripConfig.width;

        int borderThickness = 2; // Thickness of the border
        cv::Scalar borderColor = cv::Scalar(0, 0, 0); // Black border

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

        // Create a larger canvas
        int canvasWidth = frame.cols + verticalLedSize * 2 + borderThickness * 2;
        int canvasHeight = frame.rows + horizontalLedSize * 2 + borderThickness * 2;
        m_canvas = cv::Mat(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(0, 0, 0));

        // Check if canvas is valid
        if (m_canvas.empty()) {
            throw std::runtime_error("Canvas is empty after initialization");
        }

        // Copy the frame with border to the center of the canvas
        frameWithBorder.copyTo(m_canvas(cv::Rect(verticalLedSize, horizontalLedSize, frameWithBorder.cols, frameWithBorder.rows)));

        // Draw Top, Bottom
        int offset_t = m_stripConfig.height;
        int offset_b = m_stripConfig.height * 2 + m_stripConfig.width;
        for (int i = 0; i < m_stripConfig.width; ++i) {
            // Top side
            cv::rectangle(
                m_canvas,
                // Area
                cv::Rect(
                    i * horizontalLedSize + verticalLedSize + borderThickness,
                    0,
                    horizontalLedSize,
                    horizontalLedSize
                ),
                // Color
                cv::Scalar(
                    m_pColorArray[i + offset_t] & 0xFF,
                    (m_pColorArray[i + offset_t] >> 8) & 0xFF,
                    (m_pColorArray[i + offset_t] >> 16) & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                m_canvas,
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
            if (m_stripConfig.bottom) {
                int i_inverse = m_stripConfig.width - 1 - i;
                cv::rectangle(
                    m_canvas,
                    // Area
                    cv::Rect(
                        i_inverse * horizontalLedSize + verticalLedSize + borderThickness,
                        canvasHeight - horizontalLedSize,
                        horizontalLedSize,
                        horizontalLedSize
                    ),
                    // Color
                    cv::Scalar(
                    m_pColorArray[i + offset_b] & 0xFF,
                    (m_pColorArray[i + offset_b] >> 8) & 0xFF,
                    (m_pColorArray[i + offset_b] >> 16) & 0xFF
                    ),
                    cv::FILLED
                );
                // Border
                cv::line(
                    m_canvas,
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
        int offset_r = m_stripConfig.height + m_stripConfig.width;
        for (int i = 0; i < m_stripConfig.height; ++i) {
            // Left side
            int i_inverse = m_stripConfig.height - 1 - i;
            cv::rectangle(
                m_canvas,
                // Area
                cv::Rect(
                    0,
                    i_inverse * verticalLedSize + horizontalLedSize + borderThickness,
                    verticalLedSize,
                    verticalLedSize
                ),
                // Color
                cv::Scalar(
                    m_pColorArray[i + offset_l] & 0xFF,
                    (m_pColorArray[i + offset_l] >> 8) & 0xFF,
                    (m_pColorArray[i + offset_l] >> 16) & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                m_canvas,
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
                m_canvas,
                // Area
                cv::Rect(
                    canvasWidth - verticalLedSize,
                    i * verticalLedSize + horizontalLedSize + borderThickness,
                    verticalLedSize,
                    verticalLedSize
                ),
                // Color
                cv::Scalar(
                    m_pColorArray[i + offset_r] & 0xFF,
                    (m_pColorArray[i + offset_r] >> 8) & 0xFF,
                    (m_pColorArray[i + offset_r] >> 16) & 0xFF
                ),
                cv::FILLED
            );

            // Border
            cv::line(
                m_canvas,
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

void SimulationController::resizeFrame(const cv::Mat& frame, cv::Mat& resizedFrame, cv::Size targetSize)
{
    // Resize the frame to target resolution
    cv::resize(frame, resizedFrame, targetSize);
}

cv::Mat SimulationController::createSolidColorFrame(int width, int height, uint32_t color) {
    cv::Mat frame(height, width, CV_8UC3, cv::Scalar((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF));
    return frame;
}
