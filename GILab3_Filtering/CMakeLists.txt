cmake_minimum_required(VERSION 3.16)
project(Filtering)

set(CMAKE_CXX_STANDARD 14)
# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Filtering main.cpp filtering.cpp filtering.h)

target_link_libraries(Filtering ${OpenCV_LIBS})