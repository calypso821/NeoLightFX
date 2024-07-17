cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather all source files in the src directory
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Add the source files to the example application executable
#add_executable(runExampleApp examples/main.cpp ${SOURCES})
add_executable(runExampleApp examples/main.cpp src/LEDColorController.cpp)
add_executable(runRpi3App examples/rpi3.cpp src/LEDColorController.cpp)
