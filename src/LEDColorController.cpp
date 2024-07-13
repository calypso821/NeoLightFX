#include "LEDColorController.h"

LEDColorController::LEDColorController(int width, int height, int brightness) {
    this->width = width;
    this->height = height;
    this->brightness = brightness;
    this->led_array = array_init();
}

LEDColorController::~LEDColorController() {
    free(led_array);
}

void LEDColorController::setColorProcessor(FrameProcessor *processor) {
    this->processor = processor;
}

void LEDColorController::setStaticColor(StaticColor *staticColor) {
    this->staticColor = staticColor;
}

ws2811_led_t* LEDColorController::getColorArray() {
    return led_array;
}

void LEDColorController::processFrame(cv::Mat frame) {
    processor->call(frame, led_array);
}

void LEDColorController::setStaticColorByString(std::string color) {
    staticColor->setColorByString(color, led_array);
}

void LEDColorController::setStaticColorByHex(uint32_t color) {
    staticColor->setColorByHex(color, led_array);
}

ws2811_led_t* LEDColorController::array_init() {
    return static_cast<ws2811_led_t*>(malloc(sizeof(ws2811_led_t) * LED_COUNT));
}
