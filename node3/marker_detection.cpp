#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/aruco.hpp"

cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
cv::aruco::ArucoDetector detector(dictionary, detectorParams);

cv::Mat detect_markers(cv::Mat image){
  cv::Mat copy;
  image.copyTo(copy);
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  detector.detectMarkers(image, corners, ids, rejected);
  // if at least one marker detected
  if (ids.size() > 0)
      cv::aruco::drawDetectedMarkers(copy, corners, ids);

  return copy;
}