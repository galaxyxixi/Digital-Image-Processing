cmake_minimum_required(VERSION 3.16)
project(Denoising)

set(CMAKE_CXX_STANDARD 14)
# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Denoising main.cpp denoising.cpp denoising.h)
target_link_libraries(Denoising ${OpenCV_LIBS})