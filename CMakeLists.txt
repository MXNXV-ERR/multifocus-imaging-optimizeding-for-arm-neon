cmake_minimum_required(VERSION 3.5)
project(multi_focus)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES src/main.cpp
        src/utils.h
        src/splining.cpp src/splining.h
#src/tiling.cpp src/tiling.h
        src/custom_multifocus.cpp src/custom_multifocus.h
        src/alignment.cpp src/alignment.h
	src/NEON_2_SSE.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(multi_focus ${SOURCE_FILES})


# EXTERNAL LIBRAIRIES =====================

# OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(multi_focus ${OpenCV_LIBS})