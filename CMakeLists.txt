project( Simulator )
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_FLAGS "--std=c++11 -Wall -pedantic")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(rect rect/rect.cpp rect/rect.h)
include_directories(rect/)

add_library(ball ball/ball.cpp ball/ball.h)
include_directories(ball/)
target_link_libraries(ball ${OpenCV_LIBS} rect)

add_library(controller controller/controller.cpp controller/controller.h)
include_directories(controller/)
target_link_libraries(controller ${OpenCV_LIBS} ball)

add_executable(simulator simulator.cpp)
target_link_libraries(simulator ${OpenCV_LIBS} rect ball controller)
