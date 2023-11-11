
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "marker_detection.h"

using namespace cv;
int main()
{
    cv::VideoCapture cap(0);

    if (!cap.isOpened()){
      std::cerr << "Error: Camera could not be opened!" << std::endl;
    }
    while (true){
      cv::Mat frame;
      cap >> frame;

      if (frame.empty()) break;

      cv::Mat gray;
      cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
      cv::Mat binary;
      cv::threshold(gray, binary, 200, 255, cv::THRESH_BINARY);

      std::vector<std::vector<cv::Point>> contours;
      std::vector<cv::Vec4i> hierarchy;
      cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
      cv::Point2f center;
      float radius;

    // Draw contours on the original image
      cv::Mat imageWithContours = gray.clone(); // Clone the original image if you don't want to modify it

      for (size_t i = 0; i < contours.size(); i++) {
          cv::Scalar color = cv::Scalar(0, 255, 0); // Green color for the contours
          cv::drawContours(imageWithContours, contours, static_cast<int>(i), color, 2, cv::LINE_8, hierarchy, 0);
          //cv::minEnclosingCircle(contours[i], center, radius);
          //cv::circle(imageWithContours, center, radius, cv::Scalar(0, 0, 255) );
      }
            

      cv::Mat markers = detect_markers(imageWithContours);
      cv::imshow("Frame", markers);
      
      if(cv::waitKey(1) >= 0) break;
    }
    cap.release();
    cv::destroyAllWindows();
}

