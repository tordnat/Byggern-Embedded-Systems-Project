#pragma once
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>

#define MARKER_ID_TOP_LEFT      40
#define MARKER_ID_TOP_RIGHT     124
#define MARKER_ID_BOTTOM_LEFT   23
#define MARKER_ID_BOTTOM_RIGHT  62
#define MARKER_ID_ACTUATOR      203

typedef struct{
    float id;
    std::vector<cv::Point2f> corner;
} marker_t;

