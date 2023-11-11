
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "serial_transmit.h"
#include "marker_calibration.h"

using namespace cv;

void visualizeBallTracking(cv::Mat& frame, const cv::Point2f& ballPosition) {
    if (ballPosition.x >= 0 && ballPosition.y >= 0) {
        // Draw a circle at the ball position
        const int radius = 70; // Radius of the circle
        const cv::Scalar color(0, 255, 0); // Green color
        const int thickness = 2; // Thickness of the circle outline

        cv::circle(frame, ballPosition, radius, color, thickness);

        // Optional: Add text to show the coordinates
        std::string positionText = "X: " + std::to_string(int(ballPosition.x)) +
                                   ", Y: " + std::to_string(int(ballPosition.y));
        cv::putText(frame, positionText, ballPosition + cv::Point2f(10, 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }
}

cv::Point2f detectBall(const cv::Mat& frame) {
    // Convert to HSV color space
    cv::Mat hsv;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // Define the color range for the white ball
    cv::Scalar lowerWhite(0, 0, 210); // Lower boundary for white
    cv::Scalar upperWhite(180, 50, 255); // Upper boundary for white
    cv::Mat mask;
    cv::inRange(hsv, lowerWhite, upperWhite, mask);

    // Noise reduction using morphological operations
    cv::erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter contours based on circularity
    for (const auto& contour : contours) {
        float area = cv::contourArea(contour);
        if (area > 200) { // Area threshold
            float perimeter = cv::arcLength(contour, true);
            float circularity = 4 * M_PI * area / (perimeter * perimeter);

            if (circularity > 0.8) { // Adjust this threshold for circularity
                cv::Moments M = cv::moments(contour);
                if (M.m00 != 0) {
                    cv::Point2f center(M.m10 / M.m00, M.m01 / M.m00);
                    return center; // Assuming the most circular contour is the ball
                }
            }
        }
    }
    return cv::Point2f(-1, -1); // Return an invalid point if not found
}


int main() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()){
    std::cerr << "Error: Camera could not be opened!" << std::endl;
  }

  // Initialization and calibration...
  cv::Point2f ball_distance_vector;
  // Game loop
    while (1) {
      cv::Point2f ball;
      cv::Mat frame;
      cap >> frame;
      ball = detectBall(frame);
      visualizeBallTracking(frame, ball);
      
      //string msg = std::format("Ball Distance Vector: {},{}", ball_distance_vector.x, ball_distance_vector.y)
      //serial_send("Ball Distance Vector: {%i,%i}")
      cv::imshow("Frame", frame);
      if(cv::waitKey(1) >= 0) break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
