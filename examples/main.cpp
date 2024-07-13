#include "LEDColorController.h"
#include "FrameProcessor.h"
#include "StaticColor.h"
#include "LEDStripController.h"
#include "VideoController.h"

int main() {
    // Initialize LEDColorController with the desired width, height, and brightness
    LEDColorController ledColorController(640, 480, 255);

    // Initialize FrameProcessor and StaticColor with the same width and height
    FrameProcessor frameProcessor(640, 480);
    StaticColor staticColor(640, 480);

    // Initialize LEDStripController with the desired brightness
    LEDStripController ledStripController(255);

    // Set the FrameProcessor and StaticColor in the LEDColorController
    ledColorController.setColorProcessor(&frameProcessor);
    ledColorController.setStaticColor(&staticColor);

    // Initialize VideoController with the video source and the LEDColorController
    VideoController videoController("0", &ledColorController);

    // Start the video processing
    videoController.start();

    return 0;
}
