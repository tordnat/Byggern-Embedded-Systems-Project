#pragma once

cv::Mat detect_markers(cv::Mat image);
bool loadCameraCalibration(std::string filename, cv::Mat &cameraMatrix, cv::Mat &distCoeffs);
