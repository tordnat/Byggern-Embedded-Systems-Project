
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include "serial_transmit.h"
#include "marker_calibration.h"

using namespace cv;

cv::Rect crop_rect;

cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
cv::aruco::ArucoDetector detector(dictionary, detectorParams);

#define NUM_MARKERS 5
#define CAM_HEIGHT 1080
#define CAM_WIDTH   1920

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


void visualizeActuatorAndBall(cv::Mat& frame, const cv::Point2f& ballPosition, const cv::Point2f& actuatorCenter){
  if (actuatorCenter.x >= 0 && actuatorCenter.y >= 0) {
  visualizeBallTracking(frame, ballPosition);
  const cv::Scalar color(0, 255, 0); // Green color
  const int thickness = 2; // Thickness of the circle outline
  //cv::arrowedLine(frame, actuatorCenter, ballPosition, color, thickness, cv::LINE_8, 0, 0);
  cv::arrowedLine(frame, actuatorCenter, ballPosition, color);
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

cv::Mat cropFrameByMarkers(const cv::Mat& frame, 
                           const std::vector<int> markerIds,
                           const std::vector<cv::Point2f> centers) {
    // Define points for each corner marker
    if(markerIds.size() != NUM_MARKERS)  return frame;
    cv::Point2f topLeft, topRight, bottomLeft, bottomRight;

    for (size_t i = 0; i < markerIds.size(); i++) {

        // Assign center to corresponding variable based on marker ID
        switch(markerIds[i]) {
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
            // Add case for MARKER_ID_ACTUATOR if needed
        }
    }

    // Initialize variables to the first center's coordinates
    float minX = centers[0].x;
    float minY = centers[0].y;
    float maxX = centers[0].x;
    float maxY = centers[0].y;

    // Process each marker
    for (size_t i = 0; i < markerIds.size(); i++) {
        const cv::Point2f& center = centers[i];

        // Update the min and max values
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

    cv::Rect cropRect(minX, minY, width, height);
    crop_rect = cropRect; //Update global crop rect
    // Check if the cropRect is entirely within the frame bounds
    if(minX < 0 || minY < 0 || maxX > frame.cols || maxY > frame.rows) {
        std::cerr << "Cropping rectangle is out of bounds." << std::endl;
        return frame;
    }


    cv::Mat croppedFrame;
    // Safely create the cropped frame
    if (cropRect.width > 0 && cropRect.height > 0) {
        croppedFrame = frame(cropRect);
    }

    return croppedFrame;
}

cv::Point2f get_actuator_center(const std::vector<int> markerIds, const std::vector<cv::Point2f> centers){
  for (size_t i = 0; i < markerIds.size(); i++) {
    if (markerIds[i] == MARKER_ID_ACTUATOR){
      return centers[i];
    }      
  }
  return cv::Point2f(-1, -1);
}

cv::Point2f adjustForCropping(const cv::Point2f& pointInCroppedFrame, const cv::Rect& cropRect) {
    if (pointInCroppedFrame.x < 0 || pointInCroppedFrame.y < 0){
      return pointInCroppedFrame;
    }
    return cv::Point2f(pointInCroppedFrame.x + cropRect.x, pointInCroppedFrame.y + cropRect.y);
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
      cv::Point2f prev_ball;
      cv::Point2f actuator_center;
      cv::Mat frame;

      cap >> frame;
      if (frame.empty()) {
        std::cout << "Frame is empty" << std::endl;
      }
      
      //string msg = std::format("Ball Distance Vector: {},{}", ball_distance_vector.x, ball_distance_vector.y)
      //serial_send("Ball Distance Vector: {%i,%i}")

      std::vector<int> markerIds;
      std::vector<std::vector<cv::Point2f>> markerCorners;
      std::vector<std::vector<cv::Point2f>> rejected;
      std::vector<cv::Point2f> centers;
    
      detector.detectMarkers(frame, markerCorners, markerIds, rejected);
      if (markerIds.size() == NUM_MARKERS){
      for (auto& corners : markerCorners) {
        cv::Point2f center(0, 0);
        for (auto& corner : corners) {
            center.x += corner.x;
            center.y += corner.y;
        }
        center.x /= corners.size();
        center.y /= corners.size();
        centers.push_back(center);
      }
      cv::aruco::drawDetectedMarkers(frame, markerCorners, markerIds);
        for (int i = 0; i < markerCorners.size(); i++) {
            cv::circle(frame, centers[i], 3, cv::Scalar(0, 0, 255), -1);
          }


      cv::Mat cropped_frame = cropFrameByMarkers(frame, markerIds, centers);
      ball = detectBall(cropped_frame);
      actuator_center = get_actuator_center(markerIds, centers);
      if ((ball != cv::Point2f(-1, -1) )&& (actuator_center != cv::Point2f(-1, -1))){
              ball = adjustForCropping(ball, crop_rect);
              visualizeActuatorAndBall(frame, ball, actuator_center);

      }
      }
      cv::imshow("Frame", frame);
      if(cv::waitKey(1) >= 0) break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}
