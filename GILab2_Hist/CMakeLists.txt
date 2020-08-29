cmake_minimum_required(VERSION 3.16)
project(Hist)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Hist main.cpp Hist.cpp Hist.h)
target_link_libraries(Hist ${OpenCV_LIBS})