cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/rpi3)
include_directories(${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x)

# Gather all source files in the src directory
# file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Sources
set(SOURCES
    ${PROJECT_SOURCE_DIR}/src/LEDColorController.cpp
    ${PROJECT_SOURCE_DIR}/rpi3/LEDStripControllerRpi3.cpp
)

# Add the submodule source files
set(SUBMODULE_SOURCES
    # Exmaple 
    # ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/ws2811.c
)

# Add the rpi_ws281x source files
set(RPI_WS281X_SOURCES
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/ws2811.c
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/rpihw.c
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/pwm.c
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/dma.c
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/pcm.c
    ${PROJECT_SOURCE_DIR}/rpi3/third_party/rpi_281x/mailbox.c
)

# Create a static library from the submodule source files
add_library(rpi_ws281x STATIC ${RPI_WS281X_SOURCES})

# Exmaple executable sources
set(EX_MAIN_CPP ${PROJECT_SOURCE_DIR}/example/main.cpp)
set(EX_RPI3_CPP ${PROJECT_SOURCE_DIR}/example/rpi3.cpp)

# Add the source files to the example application executable
add_executable(runExampleApp ${EX_MAIN_CPP} ${SOURCES} ${SUBMODULE_SOURCES})
add_executable(runRpi3App ${EX_RPI3_CPP} ${SOURCES} ${SUBMODULE_SOURCES})

# Link the static library with the example application executables
target_link_libraries(runRpi3App rpi_ws281x)
