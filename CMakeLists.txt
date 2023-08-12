cmake_minimum_required(VERSION 3.2)
project(multi_focus)


set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_GENERATOR_PLATFORM ARM64 CACHE INTERNAL "")
set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)
set(CMAKE_FIND_ROOT_PATH "C:/Users/MXNXV-ERR/Downloads/llvm-mingw-20230614-msvcrt-i686/bin")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES src/main.cpp
        src/utils.h
        src/splining.cpp src/splining.h
        src/tiling.cpp src/tiling.h
        src/custom_multifocus.cpp src/custom_multifocus.h
        src/alignment.cpp src/alignment.h)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(multi_focus ${SOURCE_FILES})


# EXTERNAL LIBRAIRIES =====================

# OpenCV
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(multi_focus ${OpenCV_LIBS})

# find_package(OpenMP REQUIRED)
# target_link_libraries(multi_focus PRIVATE OpenMP::OpenMP_CXX)