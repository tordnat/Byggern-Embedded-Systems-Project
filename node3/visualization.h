#pragma once
#include <opencv2/core/types.hpp>

void visualize_ball_tracking(cv::Mat& frame, const cv::Point2f& ball_position);
void visualize_ball_and_actuator(cv::Mat& frame, const cv::Point2f& ball_position, const cv::Point2f& actuator_center);
cv::Mat crop_frame_by_markers(const cv::Mat& frame, const std::vector<int> marker_ids, const std::vector<cv::Point2f> centers, cv::Rect &crop_rect);
cv::Point2f adjust_for_cropping(const cv::Point2f& point_in_cropped_frame, const cv::Rect& cropping_rectangle);