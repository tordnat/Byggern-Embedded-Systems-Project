#include <opencv2/core.hpp>
#include <iostream>
#include <string.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include "serial_transmit.h"
#include "marker_tracking.h"
#include "visualization.h"

using namespace cv;

cv::Point2f detect_ball(const cv::Mat& frame){
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV); //Convert to HSV

    cv::Scalar lower_white_bound(0, 0, 210); //Lower boundary for white ball
    cv::Scalar upper_white_bound(180, 50, 255); //Upper boundary for white ball
    cv::Mat mask;
    cv::inRange(hsv, lower_white_bound, upper_white_bound, mask);

    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2); //Noise reduction
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    for (const auto& contour : contours) {//Filter contours based on circularity
        float area = cv::contourArea(contour);
        if (area > 200) { // Area threshold
            float perimeter = cv::arcLength(contour, true);
            float circularity = 4 * M_PI * area / (perimeter * perimeter);

            if (circularity > 0.82) {
                cv::Moments M = cv::moments(contour);
                if (M.m00 != 0) {
                    cv::Point2f center(M.m10 / M.m00, M.m01 / M.m00);
                    return center; //Assuming the most circular contour the ball 👍
                }
            }
        }
    }
    return cv::Point2f(-1, -1); //Return invalid point if not found
}

cv::Point2f get_actuator_center(const std::vector<int> marker_ids, const std::vector<cv::Point2f> centers){
  for (size_t i = 0; i < marker_ids.size(); i++) {
    if (marker_ids[i] == MARKER_ID_ACTUATOR){
      return centers[i];
    }      
  }
  return cv::Point2f(-1, -1);
}
