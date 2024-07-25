cmake_minimum_required(VERSION 3.10)
project(NeoLightFX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the path to the OpenCVConfig.cmake file
set(OpenCV_DIR "D:/repos/libs/opencv/build/x64/vc16/lib")
# Find OpenCV package
find_package(OpenCV REQUIRED)


# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Gather all source files in the src directory
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

set(SOURCES 
    src/controllers/LEDColorController.cpp
    src/controllers/InputSourceController.cpp
    src/utils/SimulationUtils.cpp
    src/sources/VideoSource.cpp
    src/sources/ImageSource.cpp
    src/FrameProcessor.cpp
    src/RenderManager.cpp
    src/Debug.cpp
)

# Add the source files to the example application executable

add_executable(NeoLightFX_Simulator
    main/main.cpp
    ${SOURCES}
)
target_link_libraries(NeoLightFX_Simulator ${OpenCV_LIBS})
#[[
add_executable(runExampleApp 
    example/main.cpp 
    ${SOURCES}
)

add_executable(simulationApp 
    simulation/led_simulation.cpp
    ${SOURCES}
)

add_executable(runInputSource 
    example/inputSource.cpp
    ${SOURCES}
)

# Link OpenCV libraries using absolute path
# target_link_libraries(runInputSource ${OpenCV_LIBS})
# target_link_libraries(runExampleApp ${OpenCV_LIBS})
# target_link_libraries(simulationapp ${opencv_libs})
]]

# Add opencv.dll files to PATH!!
# D:\repos\libs\opencv\build\x64\vc16\bin

