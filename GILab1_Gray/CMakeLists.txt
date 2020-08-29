cmake_minimum_required(VERSION 3.16)
project(Gray)


set(CMAKE_CXX_STANDARD 14)
# 添加OpenCV库
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS} Gray.h)

add_executable(Gray main.cpp Gray.cpp)
target_link_libraries(Gray ${OpenCV_LIBS})