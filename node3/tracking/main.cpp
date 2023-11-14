#include <_types/_uint16_t.h>
#include <_types/_uint32_t.h>
#include <opencv2/core.hpp>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc.hpp>
#include "serial_transmit.h"
#include "visualization.h"
#include "ball_tracking.h"
#include "marker_tracking.h"

/* This program heavily utilizes OpenCV Tutorials
* - https://docs.opencv.org/4.x/d9/df8/tutorial_root.html
*/

#define MARKER_ID_ACTUATOR      0

int main() {
    //Set up serial
    /* Taken from Apple forums
    * - https://developer.apple.com/forums/thread/697472
    */

    boost::asio::io_service io;
    boost::asio::serial_port sp(io, "/dev/tty.usbserial-1420");

    sp.set_option(boost::asio::serial_port_base::baud_rate(115200));

    boost::system::error_code ec;
    std::string serial_data;


  cv::VideoCapture cap(0);
  //cv::VideoCapture cap("../test_data/tracking_test2.mp4"); // Testing
  if (!cap.isOpened()){
    std::cerr << "Error: Camera could not be opened!" << std::endl;
  }
    cv::Mat frame;
    cv::Mat cropped_frame;
    cv::Rect cropping_rectangle;
    cv::Point2f ball_pos;
    cv::Point2f ball_pos_prev;
    cv::Point2f actuator_center;
    cv::Point2f actuator_center_prev;

    char data[10];
    // ArUco Tracker Detection variables
    std::vector<int> marker_ids;
    std::vector<std::vector<cv::Point2f>> marker_corners;
    std::vector<cv::Point2f> centers;

    while (1) {
        cap >> frame;
        if (frame.empty()) {
        std::cout << "Frame is empty!" << std::endl;
        }
        ball_pos = detect_ball(frame);

        if((ball_pos != cv::Point2f(-1, -1)) && ((ball_pos.x > 765) && (ball_pos.x < 1600))){
            visualize_ball_tracking(frame, ball_pos);
            serial_data = std::to_string(ball_pos.x) + "," + std::to_string(ball_pos.y) + '\0';
            boost::asio::write(sp, boost::asio::buffer(serial_data), ec);
            // Read from the serial port
            size_t len = sp.read_some(boost::asio::buffer(data), ec);
            std::cout.write(data, len);
            std::cout << serial_data << std::endl;
            }
        cv::imshow("Byggern' Ballz", frame);
        if(cv::waitKey(1) >= 0) break;
        }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}