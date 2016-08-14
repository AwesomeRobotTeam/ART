#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "RecordVideo.h"

/*Termio*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
//#include "term2Arduino.h"

/*ROS*/
#include "ros_api.h"
#include "msg.h"

using namespace std;

int main(int argc, char *argv[]) {
	//	if (recordVideo() == 1)
	//		cout << "Recorded successfully" << endl;
	//	else
	//		cout << "Failed to record" << endl;

	if (argc != 3)
		return -1;

	cv::VideoCapture myVideoCapture(0);
	if (!myVideoCapture.isOpened())
		return -1;

	cv::namedWindow("Video Captured");

/*ROS*/

	r_init( CameraDetectEdge);
	r_hdlr( hdl);
	r_newPub( pub, hdl, challenge1::direction, direction, 100);
	ros::Rate loop_rate(10);
	m_dir dir;
	char c[2]="c";

/*******<Termio>*******/
/*	int fd;
	char bridge[20];
	strcpy( bridge, defaultBridge);

	speed_t baudrate = B9600;
  p_termios option = (p_termios) malloc( sizeof( struct termios));
  fd = shake_hand( bridge, baudrate, option);
	char c[2] = "c";*/
/******<\Termio>*******/

	int timer = 0;
	while (ros::ok()) {//
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) ///<*** 'Esc' key
		{
			c[0] = 's';
			/*host_write( fd, c, 1);*/
			dir.direction= (int) c[0];
			break;
		}

		cv::Mat inputFrame;
		myVideoCapture >> inputFrame;
		resize(inputFrame, inputFrame, cv::Size(inputFrame.cols / 3, inputFrame.rows / 3));
		cv::Mat edges;
		Canny(inputFrame, edges, atoi(argv[1]), atoi(argv[2])); // 150 200
		cv::imshow("Video Captured", edges);

		timer++;
		if (timer > 10) {
			timer = 0;
			continue;
		}

		int left_cnt = 0;
		int right_cnt = 0;
		for (int i = 0; i < edges.rows; i++) {
			int white_pos = 0;
			int white_cnt = 0;
			for (int j = 0; j < edges.cols; j++) {
				if (edges.at<uchar>(i, j) == 255) {
					white_pos += j;
					white_cnt++;
				}
			}

			int avg_pos = 0;
			if (white_cnt > 0) {
				avg_pos = white_pos / white_cnt;
				right_cnt += avg_pos;
			}
			left_cnt += edges.cols - avg_pos;
		}

		float left_ratio = left_cnt / float(edges.rows * edges.cols);
		float right_ratio = right_cnt / float(edges.rows * edges.cols);
		if (left_ratio > right_ratio && (left_ratio - right_ratio) > 0.4)
		{
			cout << "Turn right" << endl;
			c[0] = 'r';
			/*host_write( fd, c, 1);*/
			dir.direction= (int) c[0];
		}
		else if (right_ratio > left_ratio && (right_ratio - left_ratio) > 0.4)
		{
			cout << "Turn left" << endl;
			c[0] = 'l';
			/*host_write( fd, c, 1);*/
			dir.direction= (int) c[0];
		}
		else
		{
			cout << "Go straight" << endl;
			c[0] = 'f';
			/*host_write( fd, c, 1);*/
			dir.direction= (int) c[0];
		}
		/**/
		pub.publish( dir);
		ros::spinOnce();
		loop_rate.sleep();
	}

	return 0;
}
