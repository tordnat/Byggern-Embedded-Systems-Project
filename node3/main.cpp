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


int main() {
    //Set up serial
    /* Taken from Apple forums
    * - https://developer.apple.com/forums/thread/697472
    */

    boost::asio::io_service io;
    boost::asio::serial_port sp(io, "/dev/tty.usbserial-11440");

    sp.set_option(boost::asio::serial_port_base::baud_rate(115200));

    char data[10];
    boost::system::error_code ec;


  //cv::VideoCapture cap(0);
  cv::VideoCapture cap("../test_data/tracking_test2.mp4"); // Testing
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

    // ArUco Tracker Detection variables
    std::vector<int> marker_ids;
    std::vector<std::vector<cv::Point2f>> marker_corners;
    std::vector<cv::Point2f> centers;

    while (1) {
        cap >> frame;
        if (frame.empty()) {
        std::cout << "Frame is empty!" << std::endl;
        }
        
        detect_markers_center(frame, marker_ids, marker_corners, centers);
        if(marker_ids.size() >= NUM_MARKERS-1){
            cropped_frame = crop_frame_by_markers(frame, marker_ids, centers, cropping_rectangle);
            ball_pos = detect_ball(cropped_frame);
            actuator_center = get_actuator_center(marker_ids, centers);
            draw_markers(frame, marker_ids, marker_corners, centers);
            if ((ball_pos != cv::Point2f(-1, -1) ) && (actuator_center != cv::Point2f(-1, -1))){
                    visualize_ball_and_actuator(frame, adjust_for_cropping(ball_pos, cropping_rectangle), actuator_center);
                    actuator_center_prev = actuator_center;

                    int ball_to_actuator_x_dist = ball_pos.y;
                    boost::asio::write(sp, boost::asio::buffer(std::to_string(ball_to_actuator_x_dist)), ec);
                    // Read from the serial port
                    //size_t len = sp.read_some(boost::asio::buffer(data), ec);
                    //std::cout.write(data, len);
                    
            } else if(ball_pos != cv::Point2f(-1, -1)){
                visualize_ball_and_actuator(frame, adjust_for_cropping(ball_pos, cropping_rectangle), actuator_center_prev);
            }
        }
        cv::imshow("Byggern' Ballz", frame);
        if(cv::waitKey(1) >= 0) break;
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}