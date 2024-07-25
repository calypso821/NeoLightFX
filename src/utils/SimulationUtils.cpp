#include "utils/SimulationUtils.h"

// Function to simulate the LED display around the frame
void simulateLEDs(const cv::Mat& frame, const uint32_t* colorArray, int ledNumWidth, int ledNumHeight, bool showBottom) {
    int width = frame.cols;
    int height = frame.rows;

    int verticalLedSize = height / ledNumHeight;
    int horizontalLedSize = width / ledNumWidth;

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

    // Create a larger canvas
    int canvasWidth = frame.cols + verticalLedSize * 2 + borderThickness * 2;
    int canvasHeight = frame.rows + horizontalLedSize * 2 + borderThickness * 2;
    cv::Mat canvas(canvasHeight, canvasWidth, CV_8UC3, cv::Scalar(0, 0, 0));

    // Copy the frame with border to the center of the canvas
    frameWithBorder.copyTo(canvas(cv::Rect(verticalLedSize, horizontalLedSize, frameWithBorder.cols, frameWithBorder.rows)));

    // Draw Top, Bottom
    int offset_t = ledNumHeight;
    int offset_b = ledNumHeight * 2 + ledNumWidth;
    for (int i = 0; i < ledNumWidth; ++i) {
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
            int i_inverse = ledNumWidth - 1 - i;
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
    int offset_r = ledNumHeight + ledNumWidth;
    for (int i = 0; i < ledNumHeight; ++i) {
        // Left side
        int i_inverse = ledNumHeight - 1 - i;
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

// Function to create a solid color frame
cv::Mat createSolidColorFrame(int width, int height, uint32_t color) {
    cv::Mat frame(height, width, CV_8UC3, cv::Scalar((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF));
    return frame;
}
