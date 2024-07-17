cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/rpi3)

# Gather all source files in the src directory
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Sources
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/LEDColorController.cpp
    ${PROJECT_SOURCE_DIR}/rpi3/LEDStripControllerRpi3.cpp
)

# Add the source files to the example application executable
add_executable(runExampleApp example/main.cpp ${SOURCES})
add_executable(runRpi3App example/rpi3.cpp ${SOURCES})
