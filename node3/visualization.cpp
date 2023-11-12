#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "marker_tracking.h"
#include "visualization.h"

using namespace cv;


cv::Mat crop_frame_by_markers(const cv::Mat& frame, const std::vector<int> marker_ids, const std::vector<cv::Point2f> centers, cv::Rect &crop_rect) {
    //Define points for each corner marker
    if(marker_ids.size() != NUM_MARKERS)  return frame;
    cv::Point2f topLeft, topRight, bottomLeft, bottomRight;

    for (size_t i = 0; i < marker_ids.size(); i++) {

        // Assign center to corresponding variable based on corner marker IDs
        switch(marker_ids[i]) {
            case MARKER_ID_TOP_LEFT:
                topLeft = centers[i];
                break;
            case MARKER_ID_TOP_RIGHT:
                topRight = centers[i];
                break;
            case MARKER_ID_BOTTOM_LEFT:
                bottomLeft = centers[i];
                break;
            case MARKER_ID_BOTTOM_RIGHT:
                bottomRight = centers[i];
                break;
        }
    }

    float minX = centers[0].x;
    float minY = centers[0].y;
    float maxX = centers[0].x;
    float maxY = centers[0].y;

    // Process each marker
    for (size_t i = 0; i < marker_ids.size(); i++) {
        const cv::Point2f& center = centers[i];
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }

    // Use these points to define the cropping rectangle
    minX = std::max(minX, 0.f);
    minY = std::max(minY, 0.f);
    maxX = std::min(maxX, static_cast<float>(frame.cols));
    maxY = std::min(maxY, static_cast<float>(frame.rows));
    float width = maxX - minX;
    float height = maxY - minY;

    if(width <= 0 || height <= 0) {
        std::cerr << "Invalid cropping rectangle dimensions." << std::endl;
        return frame; // Return the original frame or handle the error as appropriate
    }

    cv::Rect cropping_rectangle(minX, minY, width, height);
    crop_rect = cropping_rectangle; //Update crop_rect

    if(minX < 0 || minY < 0 || maxX > frame.cols || maxY > frame.rows) {
        std::cerr << "Cropping rectangle is out of bounds." << std::endl;
        return frame;
    }


    cv::Mat croppedFrame;

    if (cropping_rectangle.width > 0 && cropping_rectangle.height > 0) {
        croppedFrame = frame(cropping_rectangle);
    }

    return croppedFrame;
}

cv::Point2f adjust_for_cropping(const cv::Point2f& point_in_cropped_frame, const cv::Rect& cropping_rectangle) {
    if (point_in_cropped_frame.x < 0 || point_in_cropped_frame.y < 0){
      return point_in_cropped_frame;
    }
    return cv::Point2f(point_in_cropped_frame.x + cropping_rectangle.x, point_in_cropped_frame.y + cropping_rectangle.y);
}


void visualize_ball_tracking(cv::Mat& frame, const cv::Point2f& ball_position) {
    if (ball_position.x >= 0 && ball_position.y >= 0) {
        // Draw a circle at the ball position
        const int radius = 70; // Radius of the circle
        const cv::Scalar color(0, 255, 0); // Green color
        const int thickness = 2; // Thickness of the circle outline

        cv::circle(frame, ball_position, radius, color, thickness);

        // Optional: Add text to show the coordinates
        std::string positionText = "X: " + std::to_string(int(ball_position.x)) +
                                   ", Y: " + std::to_string(int(ball_position.y));
        cv::putText(frame, positionText, ball_position + cv::Point2f(10, 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }
}


void visualize_ball_and_actuator(cv::Mat& frame, const cv::Point2f& ball_position, const cv::Point2f& actuator_center){
  if (actuator_center.x >= 0 && actuator_center.y >= 0) {
  visualize_ball_tracking(frame, ball_position);
  const cv::Scalar color(0, 255, 0); //green
  cv::arrowedLine(frame, actuator_center, ball_position, color);
  }
}


