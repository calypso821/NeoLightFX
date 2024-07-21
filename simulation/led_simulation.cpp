#include <opencv2/opencv.hpp>
#include <vector>
#include <cstdint>
#include <iostream>
#include <csignal>
#include <Windows.h>
//#include <unistd.h>


volatile bool keepRunning = true;

// Signal handler function
void signalHandler(int signum) {
    keepRunning = false;
}

#include "LEDColorController.h"


// Function to simulate the LED display around the frame
void simulateLEDs(const cv::Mat& frame, const uint32_t* colorArray, int ledNumWidth, int ledNumHeight, bool showBottom)
{
    int width = frame.cols;
    int height = frame.rows;

    int verticalLedSize = height / ledNumHeight;
    int horizontalLedSize = width / ledNumWidth;

    /*int borderSizeVertical = verticalLedSize * 2;
    int borderSizeHorizontal = horizontalLedSize * 2;*/
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

    // Copy the frame to the center of the canvas
    //frame.copyTo(canvas(cv::Rect(horizontalLedSize, verticalLedSize, frame.cols, frame.rows)));

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
        if (showBottom)
        {
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
        // ============ Left side =============
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

        // ============ Right side =============
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

int main() {
    // Register the signal handler
    signal(SIGINT, signalHandler);

	// Example usage
	int ledNumWidth = 10;  // Number of LEDs horizontally
	int ledNumHeight = 6; // Number of LEDs vertically
	int totalLEDs = 2 * ledNumWidth + 2 * ledNumHeight;
    bool showBottom = true;

	// Initialize LED color controller
	LEDColorController lcc = LEDColorController(ledNumWidth, ledNumHeight, showBottom);

	// Get array of colors
	uint32_t* pColorArray = lcc.getColorArray();


	// Choose between using a static color or an actual frame
	bool useStaticColor = true; // Set this to false if you want to use a real frame
	cv::Mat frame;

	if (useStaticColor) {
		uint32_t staticColor = 0xFF0000; // Example: Red color
		lcc.setColorByHexCode(staticColor);
		frame = createSolidColorFrame(320, 140, staticColor); // Create a solid color frame
	}
	else {
		frame = cv::imread("D:/resources/pictures/test.jpg"); // Replace with actual frame source
		if (frame.empty()) {
			std::cerr << "Error: Could not read the image." << std::endl;
			return -1;
		}
	}

	while (keepRunning) {
		// Process the frame with your ImageProcessor logic
		// ImageProcessor::processframe(colorArray, frame); // Uncomment and implement this line with your logic

		// Simulate the LEDs around the frame
		simulateLEDs(frame, pColorArray, ledNumWidth, ledNumHeight, showBottom);

		// Read next frame (for demonstration, just repeat the same frame)
		// frame = ...; // Replace with code to capture the next frame

        // Add a delay of 100 milliseconds (adjust as needed)
        Sleep(100); // 100 milliseconds
        //usleep(100000); // 100000 microseconds = 100 milliseconds

	}
    cv::destroyAllWindows();
	return 0;
}
