#include <cstddef>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/aruco.hpp>
#include <vector>
#include "marker_tracking.h"

using namespace cv;

cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
cv::aruco::ArucoDetector detector(dictionary, detectorParams);

void detect_markers_center(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners, std::vector<cv::Point2f> &centers){
    std::vector<std::vector<cv::Point2f>> rejected;
    detector.detectMarkers(frame, marker_corners, marker_ids, rejected);

    if (marker_ids.size() == NUM_MARKERS){ // Centering
      for (auto &corners : marker_corners) {
        cv::Point2f center(0, 0);
        for (auto& corner : corners) {
            center.x += corner.x;
            center.y += corner.y;
        }
        center.x /= corners.size();
        center.y /= corners.size();
        centers.push_back(center);
      }
      }
}

void draw_markers(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners,std::vector<cv::Point2f> &centers){
        cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);
        for (int i = 0; i < marker_corners.size(); i++) {
            cv::circle(frame, centers[i], 3, cv::Scalar(0, 0, 255), -1);
        }

}