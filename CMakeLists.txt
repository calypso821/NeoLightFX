cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add options for simulation and hardware
option(SIMULATE "Build the simulation executable" OFF)
option(HARDWARE "Build the hardware executable" OFF)

# Use case 
# cmake -DSIMULATE=ON -DHARDWARE=ON ..
# cmake -DSIMULATE=ON ..

# Set the path to the OpenCV library
if(WIN32)
    set(OpenCV_DIR "D:/repos/libs/opencv/build/x64/vc16/lib")
    # Add opencv.dll files to PATH!!
    # D:\repos\libs\opencv\build\x64\vc16\bin
elseif(UNIX)
    set(OpenCV_DIR "/usr/share/opencv4")
endif()

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party)
include_directories(${OpenCV_INCLUDE_DIRS})

# Gather all source files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/controllers/LEDColorController.cpp
    ${PROJECT_SOURCE_DIR}/src/controllers/SimulationController.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/VideoSource.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/CaptureSource.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/ImageSource.cpp
    ${PROJECT_SOURCE_DIR}/src/FrameProcessor.cpp
    ${PROJECT_SOURCE_DIR}/src/RenderManager.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/Debug.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/mode_utils.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/timer_utils.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/color_processing_utils.cpp

)

# Define a macro for conditional compilation
if(HARDWARE)
    add_compile_definitions(HARDWARE_MODE)
    set(HARDWARE_SOURCES
        ${PROJECT_SOURCE_DIR}/src/controllers/HardwareController.cpp
        ${PROJECT_SOURCE_DIR}/src/hardware/rpi3/LEDStripControllerRpi3.cpp
    )
    list(APPEND SOURCES ${HARDWARE_SOURCES})

    # Add the rpi_ws281x source files for UNIX
    if(UNIX)
        set(RPI_WS281X_SOURCES
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/ws2811.c
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/rpihw.c
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/pwm.c
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/dma.c
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/pcm.c
            ${PROJECT_SOURCE_DIR}/third_party/rpi_281x/mailbox.c
        )

        # Create a static library from the submodule source files
        add_library(rpi_ws281x STATIC ${RPI_WS281X_SOURCES})
    endif()
endif()

# SIMULATION executables
if(SIMULATE)
    add_executable(NeoLightFX_Simulator
        ${PROJECT_SOURCE_DIR}/main/main.cpp
        ${SOURCES}
    )
    target_link_libraries(NeoLightFX_Simulator PRIVATE ${OpenCV_LIBS})
    # Link against winmm.lib
    if(WIN32)
        target_link_libraries(NeoLightFX_Simulator PRIVATE winmm)
    endif()
endif()

# HARDWARE executable
if(HARDWARE AND UNIX)
    add_executable(NeoLightFX_Hardware
        ${PROJECT_SOURCE_DIR}/main/main_hardware.cpp
        ${SOURCES}
    )
    # Link the static library with the example application executables
    target_link_libraries(NeoLightFX_Hardware PRIVATE rpi_ws281x)
    target_link_libraries(NeoLightFX_Hardware PRIVATE ${OpenCV_LIBS})

    # Exmaple executables
    #[[
    add_executable(example_RPI3
        ${PROJECT_SOURCE_DIR}/example/rpi3.cpp
        ${SOURCES}
    )

    # Link the static library with the example application executables
    target_link_libraries(example_RPI3 PRIVATE rpi_ws281x)
    target_link_libraries(example_RPI3 PRIVATE ${OpenCV_LIBS})
      ]]
endif()
