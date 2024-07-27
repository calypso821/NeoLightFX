cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

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
include_directories(${PROJECT_SOURCE_DIR}/hardware)
#include_directories(${PROJECT_SOURCE_DIR}/hardware/rpi3)
#include_directories(${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x)
include_directories(${OpenCV_INCLUDE_DIRS})

# Gather all source files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(SOURCES 
    ${PROJECT_SOURCE_DIR}/src/controllers/LEDColorController.cpp
    ${PROJECT_SOURCE_DIR}/src/controllers/InputSourceController.cpp
    ${PROJECT_SOURCE_DIR}/src/controllers/SimulationController.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/VideoSource.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/CaptureSource.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/ImageSource.cpp
    ${PROJECT_SOURCE_DIR}/src/FrameProcessor.cpp
    ${PROJECT_SOURCE_DIR}/src/RenderManager.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/Debug.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/EnumToString.cpp
    ${PROJECT_SOURCE_DIR}/src/utils/percision_timer.cpp
)

# Add the source files to the example application executable
add_executable(NeoLightFX_Simulator
    main/main.cpp
    ${SOURCES}
)
target_link_libraries(NeoLightFX_Simulator PRIVATE ${OpenCV_LIBS})
# Link against winmm.lib
if(WIN32)
    target_link_libraries(NeoLightFX_Simulator PRIVATE winmm)
endif()


if(UNIX)
    # Add the rpi_ws281x source files
    set(RPI_WS281X_SOURCES
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/ws2811.c
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/rpihw.c
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/pwm.c
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/dma.c
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/pcm.c
        ${PROJECT_SOURCE_DIR}/hardware/rpi3/third_party/rpi_281x/mailbox.c
    )

    # Create a static library from the submodule source files
    add_library(rpi_ws281x STATIC ${RPI_WS281X_SOURCES})
    
    set(HARDWARE_SOURCES
        ${PROJECT_SOURCE_DIR}/hardware/LEDStripController.cpp
        ${PROJECT_SOURCE_DIR}/hardware/LEDStripControllerRpi3.cpp
    )

    # Exmaple executable sources
    set(EX_MAIN_CPP ${PROJECT_SOURCE_DIR}/example/main.cpp)
    set(EX_RPI3_CPP ${PROJECT_SOURCE_DIR}/example/rpi3.cpp)

    # Add the source files to the example application executable
    add_executable(runExampleApp ${EX_MAIN_CPP} ${SOURCES})
    add_executable(runRpi3App ${EX_RPI3_CPP} ${SOURCES} ${HARDWARE_SOURCES})

    # Link the static library with the example application executables
    target_link_libraries(runExampleApp rpi_ws281x)
    target_link_libraries(runRpi3App rpi_ws281x)
endif()
