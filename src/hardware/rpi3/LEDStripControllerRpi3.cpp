#include "hardware/rpi3/LEDStripControllerRpi3.h"

#include <iostream>
#include <cstring>

#include "utils/color_processing_utils.h"

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
                .brightness = 255,
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
        m_ledArray.channel[0].leds[x] = applyBrightnessCorrection(colorArray[x], 40);
        //m_ledArray.channel[0].leds[x] = colorArray[x];
    }
}

void LEDStripControllerRpi3::ledArrayClear()
{
    memset(m_ledArray.channel[0].leds, 0, m_ledCount * sizeof(uint32_t));
}
