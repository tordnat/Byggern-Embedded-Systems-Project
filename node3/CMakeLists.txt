# CMakeLists.txt

# Older versions of CMake are likely to work just fine but, since
# I don't know where to cut off I just use the version I'm using
cmake_minimum_required(VERSION "3.17")

# name of this example project
project(ball_tracking)



# Tell compiler to use C++ 14 features which is needed because
# Clang version is often behind in the XCode installation
set(CMAKE_CXX_STANDARD 20)

find_package( OpenCV REQUIRED )
find_package(Boost 1.65 REQUIRED COMPONENTS system)
# tell the build to include the headers from OpenCV
include_directories( ${OpenCV_INCLUDE_DIRS} )

# specify the executable target to be built
add_executable(ball_tracking ball_tracking.cpp marker_calibration.cpp serial_transmit.cpp)

# tell it to link the executable target against OpenCV
target_link_libraries(ball_tracking ${OpenCV_LIBS} )
target_link_libraries(ball_tracking Boost::system)
