#pragma once
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include "serial_transmit.h"
#include "marker_tracking.h"

cv::Point2f detect_ball(const cv::Mat& frame);
cv::Point2f get_actuator_center(const std::vector<int> marker_ids, const std::vector<cv::Point2f> centers);