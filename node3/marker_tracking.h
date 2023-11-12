#pragma once
#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>
#include <vector>

#define MARKER_ID_TOP_LEFT      40
#define MARKER_ID_TOP_RIGHT     124
#define MARKER_ID_BOTTOM_LEFT   23
#define MARKER_ID_BOTTOM_RIGHT  62
#define MARKER_ID_ACTUATOR      203

#define NUM_MARKERS 5

typedef struct{
    float id;
    std::vector<cv::Point2f> corner;
} marker_t;

void detect_markers_center(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners, std::vector<cv::Point2f> &centers);
void draw_markers(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners,std::vector<cv::Point2f> &centers);