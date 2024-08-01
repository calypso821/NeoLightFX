#include <iostream>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include "dynamicLight.h"

#define DEFAULT_COLOR       0x2F2F2F
#define BRIGHTNESS_FACTOR   50

uint8_t brightness = 130;
std::string userInput;
std::mutex inputMutex;
std::condition_variable inputCondition;
bool stopInputThread = false;
bool stopdynamicLightThread = true;
bool inputReceived = false;
std::thread dynamicLightProcessing;  // Declare the ambilight thread

void inputThread() {
    std::cout << "Input thread is running..." << std::endl;
    while (!stopInputThread) {

        std::string input;
        std::getline(std::cin, input);

        std::cout << "CPP input: " << input << std::endl;

        {
            // Lock the mutex for thread safety
            std::lock_guard<std::mutex> lock(inputMutex);

            // Store the input in the shared variable
            userInput = input;
            inputReceived = true;

            // Notify the processing thread that there's new input
            inputCondition.notify_one();
        }
    }
    std::cout << "Input thread stopped..." << std::endl;
}
VideoCapture cap;
ws2811_led_t* array1;

void video_init() {
    cap = videoCapture_init(); // video capture device initiazlie
    videoParameters_init(cap);              // video parameters initialize 
}

void led_init() {
    // INIT //
    ws2811_return_t ret;
    array1 = array_init();           // led array initialize
    ret = ledstring_init(brightness);       // led string initialize

    if (ret != WS2811_SUCCESS) {
        return;
    }
}
void led_re_init() {
    // INIT LED strip
    ws2811_fini(&ledstring);
    ledstring_init(brightness);
    // RENDER
    array_render(array1);
    ws2811_render(&ledstring);
}
void brightnessUp() {
    brightness = (brightness + BRIGHTNESS_FACTOR > 255) ? 255 : brightness + BRIGHTNESS_FACTOR;
    led_re_init();
}
void brightnessDown() {
    brightness = (brightness - BRIGHTNESS_FACTOR < 15) ? 15 : brightness - BRIGHTNESS_FACTOR;
    led_re_init();
}
void staticColor(uint32_t color) {
    std::cout << "Static color ..." << color << std::endl;

    array_setStaticColor(array1, color);
    array_render(array1);
    ws2811_render(&ledstring);
}

void dynamicLightThread() {
    std::cout << "Dynamic light thread is running..." << std::endl;
    video_init();
    cv::Mat frame;

    while (!stopdynamicLightThread) {
        // Capture frame-by-frame   
        cap >> frame;

        processFrame(array1, frame);
        array_render(array1);
        ws2811_render(&ledstring);
        // Include logic for ambilight processing here
    }
    std::cout << "Dynamic light thread stopped..." << std::endl;

    // When everything done, release the video capture object
    cap.release();

    // Closes all the frames
    destroyAllWindows();

    return;
}

int main() {
    std::thread inputProcessing(inputThread);

    led_init();
    staticColor(DEFAULT_COLOR);

    while (true) {

        // Wait for input or control the ambilight thread
        {
            std::unique_lock<std::mutex> lock(inputMutex);
            inputCondition.wait(lock, [] { return inputReceived; });

            // Process input
            if (userInput == "IR_OFF") {
                break;
            }
            // "IR_FLASH" -> Dynamic Light - ON/OFF
            if (userInput == "IR_FLASH" && stopdynamicLightThread) {
                stopdynamicLightThread = false;
                dynamicLightProcessing = std::thread(dynamicLightThread);  // Start the ambilight thread
            }
            if (userInput != "IR_FLASH" && userInput != "IR_BMINUS" && userInput != "IR_BPLUS" &&
                userInput != "IR_FADE" && userInput != "IR_SMOOTH" && !stopdynamicLightThread) {
                stopdynamicLightThread = true;
                dynamicLightProcessing.join(); // Wait for the thread to finish
            }
            if (userInput == "IR_FADE") {
                averagingDown();
            }
            if (userInput == "IR_SMOOTH") {
                averagingUp();
            }

            if (userInput == "IR_BPLUS") {
                brightnessUp();
            }
            if (userInput == "IR_BMINUS") {
                brightnessDown();
            }

            if (userInput.substr(0, 2) == "0x") {
                staticColor(std::stoul(userInput, 0, 16));
            }
            inputReceived = false;

        }

        // Other main thread logic
    }

    // Signal dynamicLight threads to stop
    stopdynamicLightThread = true;
    if (dynamicLightProcessing.joinable()) {
        dynamicLightProcessing.join();
    }

    // CLEAR ON EXIT
    array_clear(array1);
    array_render(array1);
    ws2811_render(&ledstring);

    ws2811_fini(&ledstring);    // led string finish

    std::cout << "Program close" << std::endl;

    // Signal dynamicLight threads to stop
    stopInputThread = true;
    inputCondition.notify_one();;
    inputProcessing.join(); // Wait for the thread to finish.

    return 0;
}
