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
cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
cv::aruco::ArucoDetector detector(dictionary, detectorParams);

void detect_markers_center(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners, std::vector<cv::Point2f> &centers){
    std::vector<std::vector<cv::Point2f>> rejected;
    detector.detectMarkers(frame, marker_corners, marker_ids, rejected);

    if (marker_ids.size() == NUM_MARKERS) {
        centers.resize(marker_ids.size()); // Resize centers to the number of markers
        for (size_t i = 0; i < marker_corners.size(); i++) {
            cv::Point2f center(0, 0);
            for (const auto& corner : marker_corners[i]) {
                center.x += corner.x;
                center.y += corner.y;
            }
            center.x /= marker_corners[i].size();
            center.y /= marker_corners[i].size();
            centers[i] = center; // Direct assignment instead of push_back
        }
    }
}

void draw_markers(cv::Mat &frame, std::vector<int> &marker_ids, std::vector<std::vector<cv::Point2f>> &marker_corners,std::vector<cv::Point2f> &centers){
        cv::aruco::drawDetectedMarkers(frame, marker_corners, marker_ids);
        for (int i = 0; i < marker_corners.size(); i++) {
            cv::circle(frame, centers[i], 3, cv::Scalar(0, 0, 255), -1);
        }

}