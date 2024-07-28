#include <iostream>
#include <cstring>

#include "hardware/rpi3/LEDStripControllerRpi3.h"


// WS2811 definitions
#define TARGET_FREQ             800000
#define GPIO_PIN                12
#define DMA                     10
#define STRIP_TYPE              WS2811_STRIP_GRB

LEDStripControllerRpi3::LEDStripControllerRpi3(int ledCount)
    : LEDStripController(ledCount) {} // Initialize base class 

LEDStripControllerRpi3::~LEDStripControllerRpi3()
{
    // Clean up ws2811 resources
    ws2811_fini(&m_ledArray);
    std::cout << "LED strip ws2811 resource release: Success" << std::endl;
}

void LEDStripControllerRpi3::initialize() {
    m_ledArray =
    {
        .freq = TARGET_FREQ,
        .dmanum = DMA,
        .channel =
        {
            [0] =
            {
                .gpionum = GPIO_PIN,
                .invert = 0,
                .count = m_ledCount,
                .strip_type = STRIP_TYPE,
                .brightness = 150,
            },
        },
    };
    
    ws2811_return_t ret = ws2811_init(&m_ledArray);


    if (ret != WS2811_SUCCESS) {
        throw std::runtime_error(std::string("LED strip ws2811 init failed: ")
         + ws2811_get_return_t_str(ret));
    } else {
        std::cout << "LED strip ws2811 init: " << ws2811_get_return_t_str(ret) << std::endl;
    }
    
    // Check if memory was allocated for leds
    if (!m_ledArray.channel[0].leds) {
        throw std::runtime_error("Memory allocation for LEDs: Fail");
    } else {
        std::cout << "Memory allocation for LEDs: Success" << std::endl;
    }
}

void LEDStripControllerRpi3::render() {
    ws2811_return_t ret = ws2811_render(&m_ledArray);
    //std::cout << "Redner status: " << ret << std::endl;
    
    if (ret != WS2811_SUCCESS)
    {
        fprintf(stderr, "ws2811_render failed: %s\n", ws2811_get_return_t_str(ret));
    }
}

void LEDStripControllerRpi3::ledArraySetColor(uint32_t* colorArray)
{
    for (int x = 0; x < m_ledCount; x++) {
        m_ledArray.channel[0].leds[x] = colorArray[x];
    }
}

void LEDStripControllerRpi3::ledArrayClear()
{
    memset(m_ledArray.channel[0].leds, 0, m_ledCount * sizeof(uint32_t));
}

void LEDStripControllerRpi3::setBrightness(int brightness)
{
    // TODO
}


//// WS2811 definitions
//#define TARGET_FREQ             800000
//#define GPIO_PIN                18
//#define DMA                     10
//#define STRIP_TYPE              WS2811_STRIP_GRB		// WS2812/SK6812RGB integrated chip+leds
//
//// LED definitions
//#define BOTTOM                  false   // include lower side
//
//#define LED_HEIGHT              20      // number of leds (vertical)
//#define LED_WIDTH               34      // number of leds (horizontal)
//#define LED_COUNT               (LED_HEIGHT * 2 + LED_WIDTH + LED_WIDTH * BOTTOM)
//
//#define black_bars_size         0.125   // 12.5% of height
//#define h_patch_size            0.3     // 30% of height
//#define w_patch_size            0.2     // 20% of width
//
//ws2811_t ledstring;
//ws2811_return_t ret;
//
//// ----------- INIT LED STRIP ------------ //
//ws2811_return_t ledstring_init(uint8_t brightness) {
//    ledstring =
//    {
//        .freq = TARGET_FREQ,
//        .dmanum = DMA,
//        .channel =
//        {
//            [0] =
//            {
//                .gpionum = GPIO_PIN,
//                .invert = 0,
//                .count = LED_COUNT,
//                .strip_type = STRIP_TYPE,
//                .brightness = brightness,
//            },
//            [1] =
//            {
//                .gpionum = 0,
//                .invert = 0,
//                .count = 0,
//                .brightness = 0,
//            },
//        },
//    };
//    ret = ws2811_init(&ledstring);
//
//    if (ret != WS2811_SUCCESS) {
//        fprintf(stderr, "ws2811_init failed: %s\n", ws2811_get_return_t_str(ret));
//    }
//    else {
//        fprintf(stderr, "ws2811_init: %s\n", ws2811_get_return_t_str(ret));
//    }
//    return ret;
//}
//// ----------- INIT ARRAY ------------ //
//ws2811_led_t* array_init() {
//    // led array initialize
//    return static_cast<ws2811_led_t*>(malloc(sizeof(ws2811_led_t) * LED_COUNT));
//}
//
//// ----------- RENDER ARRAY ------------ //
//void array_render(ws2811_led_t* array) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        ledstring.channel[0].leds[x] = array[x];
//    }
//}
//
//// ----------- CLEAR ARRAY ------------ //
//void array_clear(ws2811_led_t* array) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        array[x] = 0;
//    }
//}
//
//// ----------- ARRAY SET COLOR ------------ //
//void array_setStaticColor(ws2811_led_t* array, uint32_t color) {
//    int x;
//    for (x = 0; x < LED_COUNT; x++) {
//        array[x] = color;
//    }
//}
