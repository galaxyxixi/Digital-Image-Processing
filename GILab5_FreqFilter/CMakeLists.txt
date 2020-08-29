cmake_minimum_required(VERSION 3.16)
project(FreqFilter)

set(CMAKE_CXX_STANDARD 14)
# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(FreqFilter main.cpp freqFilter.cpp freqFilter.h)
target_link_libraries(FreqFilter ${OpenCV_LIBS})