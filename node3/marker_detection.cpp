#include <cstddef>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include "marker_detection.h"
#include "opencv2/aruco.hpp"

cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
cv::aruco::ArucoDetector detector(dictionary, detectorParams);

std::vector<marker_t> detect_markers(cv::Mat image){
  std::vector<int> ids;
  std::vector<std::vector<cv::Point2f>> corners, rejected;
  detector.detectMarkers(image, corners, ids, rejected);

  std::vector<marker_t> markers;
  markers.reserve(ids.size());

  for (int i = 0; i < ids.size(); ++i){
    markers[i].corner = corners[i];
    markers[i].id = ids[0];
  }

  return markers;
}

cv::Mat transform_from_markers(std::vector<marker_t> markers){
    
}