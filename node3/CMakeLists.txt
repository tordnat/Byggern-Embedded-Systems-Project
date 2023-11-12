cmake_minimum_required(VERSION "3.17")

project(ball_tracking)

set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
find_package(Boost 1.65 REQUIRED COMPONENTS system)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(ball_tracking main.cpp ball_tracking.cpp marker_tracking.cpp visualization.cpp serial_transmit.cpp)

target_link_libraries(ball_tracking ${OpenCV_LIBS} )
target_link_libraries(ball_tracking Boost::system)
