/*
 * Preferred parameters: -v 0 -s 2 -o 50 -a 60 -t 60 -r 10
 *
 * By Guan-Wen Lin
 * Last modified: Sep 19, 2016.
 *
 * ROS_wrap by Yen-Kuan Wu
 * Last modified: Sep 19, 2016.
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

/*ROS*/
#include "ros_api.h"
#include "challenge1/road.h"
/*\ROS*/

#define GO_STRAIGHT 0
#define TURN_LEFT   1
#define TURN_RIGHT  2

bool myCompFunc(cv::Vec2f a, cv::Vec2f b) {
	return a[0] < b[0];
}

cv::Mat detectRoad(cv::Mat segmentedInputFrame, int offset, int angle) {
	cv::Mat edges;
	cv::Canny(segmentedInputFrame, edges, 50, 200, 3);

	std::vector<cv::Vec2f> lines;
	cv::HoughLines(edges, lines, 1, 3 * CV_PI / 180, 100, 0, 0);

	// Sort lines by rho in ascending order
	std::sort(lines.begin(), lines.end(), myCompFunc);

	cv::Mat road = cv::Mat::zeros(edges.size(), edges.type());
	for (size_t i = 0; i < lines.size(); i++) {
		// Lines are reserved if they are far from each other and not horizontal
		if (i > 0 && (lines[i][0] - lines[i - 1][0]) < offset)
			continue;
		if (lines[i][1] > angle * CV_PI / 180 && lines[i][1] < (180 - angle) * CV_PI / 180) // 0 or 180 ~ vertical line, CV_PI/2 ~ horizontal line
			continue;

		float rho = lines[i][0], theta = lines[i][1];
		double a = cos(theta), b = sin(theta);
		double x0 = a * rho, y0 = b * rho;
		cv::Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
		cv::Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
		cv::line(road, pt1, pt2, cv::Scalar(255), 1, CV_AA);
	}

	return road;
}

int computeCenter(cv::Mat road) {
	int center = 0;

	int whitePtCount = 0;
	for (int i = 0; i < road.rows; i++) {
		for (int j = 0; j < road.cols; j++) {
			if (road.at<uchar>(i, j) > 0) {
				center += j;
				whitePtCount++;
			}
		}
	}
	if (whitePtCount > 0)
		center /= whitePtCount;

	return center;
}

int computeMovingDirection(int imgCenter, int &preCenter, int &curCenter, int movingDirection, int threshold, int resolution) {
	// If the center is undetected or there are some noises, keep the moving direction
	if (curCenter == 0)
		return movingDirection;
	if (abs(curCenter - preCenter) > threshold)
		return movingDirection;

	if (curCenter - imgCenter > resolution) {
		preCenter = curCenter;
		return TURN_RIGHT;
	}
	if (imgCenter - curCenter > resolution) {
		preCenter = curCenter;
		return TURN_LEFT;
	}
	return GO_STRAIGHT;
}

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE, SEGMENTATION_RATIO, OFFSET, ANGLE, THRESHOLD, RESOLUTION;
	while ((c = getopt(argc, argv, "v:s:o:a:t:r:")) != -1) {
		switch (c) {
			case 'v':
				DEVICE = atoi(optarg);
				break;
			case 's':
				SEGMENTATION_RATIO = atoi(optarg);
				break;
			case 'o':
				OFFSET = atoi(optarg);
				break;
			case 'a':
				ANGLE = atoi(optarg);
				break;
			case 't':
				THRESHOLD = atoi(optarg);
				break;
			case 'r':
				RESOLUTION = atoi(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-o offset] [-a angle] [-t threshold] [-r resolution]" << std::endl;
				return -1;
		}
	}
	if (argc != 13) {
		std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-o offset] [-a angle] [-t threshold] [-r resolution]" << std::endl;
		return -1;
	}

	/*ROS*/
	r_init( Road_Detecter);
	r_hdlr( hdl);
	r_newPub( pubRoad, hdl, challenge1::road, Road, 1000);
	ros::Rate loop_rate(10);

	challenge1::road rd;
	/**/

	cv::VideoCapture myVideoCapture(DEVICE);
	if (!myVideoCapture.isOpened()) {
		std::cout << "ERROR: Cannot open device " << DEVICE << std::endl;
		return -1;
	}

	cv::namedWindow("Video Captured");
	cv::namedWindow("Road Detected");

	int imgCenter = myVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH) / 2;
	int preCenter = imgCenter;
	int movingDirection = GO_STRAIGHT;
	//while (1) {
	while( ros::ok()){
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 0;

		cv::Mat inputFrame, segmentedInputFrame;
		myVideoCapture >> inputFrame;
		inputFrame(cv::Range(inputFrame.rows - inputFrame.rows / SEGMENTATION_RATIO, inputFrame.rows - 1), cv::Range::all()).copyTo(segmentedInputFrame);

		// Detect road. In general, there are only vertical lines
		cv::Mat road = detectRoad(segmentedInputFrame, OFFSET, ANGLE);

		// Compute the road center
		int curCenter = computeCenter(road);

		// Compute the moving direction
		movingDirection = computeMovingDirection(imgCenter, preCenter, curCenter, movingDirection, THRESHOLD, RESOLUTION);
		std::cout << movingDirection << std::endl;

		cv::imshow("Video Captured", segmentedInputFrame);
		cv::imshow("Road Detected", road);

		/*ROS*/
		/*switch( movingDirection){
			case GO_STRAIGHT:
			case TURN_LEFT:
			case TURN_RIGHT:
			default:
				break;
		}*/
		rd.direction = movingDirection;

		pubRoad.publish( rd);
		ros::spinOnce();
		loop_rate.sleep();
		/*\ROS*/
	}

	return 0;
}
