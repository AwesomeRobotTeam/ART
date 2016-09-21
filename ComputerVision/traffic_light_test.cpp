/*
 * Preferred parameters: -v 0 -s 3 -r 300 -d 100 -l 2
 *
 * By Guan-Wen Lin
 * Last modified: Sep 21, 2016.
 */

#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
/*ROS*/
#include "ros_api.h"
#include "challenge1/trafficLight.h"

#define GREEN 0
#define RED		1
/**/
c
cv::Mat extractRedHue(cv::Mat inputFrame_BGR) {
	// Convert input image to HSV
	cv::Mat inputFrame_HSV;
	cv::cvtColor(inputFrame_BGR, inputFrame_HSV, CV_BGR2HSV);

	// Threshold the HSV image, keep only the red pixels
	cv::Mat lowerRedHueImg;
	cv::Mat upperRedHueImg;
	cv::Mat redHueImg;
	cv::inRange(inputFrame_HSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lowerRedHueImg);
	cv::inRange(inputFrame_HSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upperRedHueImg);
	cv::addWeighted(lowerRedHueImg, 1.0, upperRedHueImg, 1.0, 0.0, redHueImg);

	return redHueImg;
}

bool findBiggestRedHue(const cv::Mat &redHueImg, const int threshold_red, cv::Point &center) {
	std::vector< std::vector<cv::Point> > contours;
	cv::findContours(redHueImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// Heuristics: If a red object is detected and close to us, the number of its contour points must be over some threshold
	int biggestIndex = -1;
	int biggestSize = 0;
	for (int i = 0; i < contours.size(); i++) {
		if (i == 0 || contours.at(i).size() > contours.at(biggestIndex).size()) {
			biggestIndex = i;
			biggestSize = contours.at(i).size();
		}
	}

	if (biggestIndex == -1 || biggestSize < threshold_red)
		return false;
	else {
		// Find the center of the red object
		center = contours.at(biggestIndex).at(0);
		for (int i = 1; i < contours.at(biggestIndex).size(); i++)
			center += contours.at(biggestIndex).at(i);
		center = cv::Point(center.x / contours.at(biggestIndex).size(), center.y / contours.at(biggestIndex).size());

		return true;
	}
}

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE, SEGMENTATION_RATIO, THRESHOLD_RED, THRESHOLD_DISTANCE;
	double LATENCY;
	while ((c = getopt(argc, argv, "v:s:r:d:l:")) != -1) {
		switch (c) {
			case 'v':
				DEVICE = atoi(optarg);
				break;
			case 's':
				SEGMENTATION_RATIO = atoi(optarg);
				break;
			case 'r':
				THRESHOLD_RED = atoi(optarg);
				break;
			case 'd':
				THRESHOLD_DISTANCE = atoi(optarg);
				break;
			case 'l':
				LATENCY = atof(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-r threshold_red] [-d threshold_distance] [-l latency]" << std::endl;
				return -1;
		}
	}
	if (argc != 11) {
		std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-r threshold_red] [-d threshold_distance] [-l latency]" << std::endl;
		return -1;
	}

	/*ROS*/
	r_init( Camera_for_Traffic_Light);
	r_hdlr( hdl);

	r_newPub( campub, hdl, challenge1::trafficLight, traf_color, 100);
	challenge1::trafficLight traf;

	ros::Rate loop_rate( 10);
	/**/

	// Start to detect the red traffic light
	cv::VideoCapture myVideoCapture(DEVICE);
	if (!myVideoCapture.isOpened()) {
		std::cout << "ERROR: Cannot open device " << DEVICE << std::endl;
		return -1;
	}

	cv::namedWindow("Video Captured");
	cv::namedWindow("Red Hue");

	bool hasDetectedRedHue = false;
	time_t start;
	cv::Point prevCenter(-1, -1);
	//while (1) {
	while( ros::ok()){
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 0;

		cv::Mat inputFrame_BGR, segmentedInputFrame_BGR;
		myVideoCapture >> inputFrame_BGR;
		inputFrame_BGR(cv::Range(0, inputFrame_BGR.rows / SEGMENTATION_RATIO), cv::Range::all()).copyTo(segmentedInputFrame_BGR);
		cv::Mat redHueImg = extractRedHue(segmentedInputFrame_BGR);

		cv::Point curCenter;
		if (findBiggestRedHue(redHueImg, THRESHOLD_RED, curCenter)) {
			// A new inco( ros::ok()){ming red object is detected
			if (!hasDetectedRedHue && cv::norm(curCenter - prevCenter) > THRESHOLD_DISTANCE) {
				std::cout << "Wait" << std::endl;
				/*ROS*/
				traf.color = RED;
				/**/

				hasDetectedRedHue = true;
				prevCenter = curCenter;
				start = clock();
			}
			// If we wait too long, the red object is not the red traffic light
			else if (((clock() - start) / (double)CLOCKS_PER_SEC) > LATENCY) {
				std::cout << "Go" << std::endl;
				/*ROS*/
				traf.color = GREEN;
				/**/
				hasDetectedRedHue = false;
				prevCenter = curCenter;
			}
			// The red object is being detected
			else
				std::cout << "Wait" << std::endl;
				/*ROS*/
				traf.color = RED;
				/**/
		}
		else {
			std::cout << "Go" << std::endl;
<<<<<<< HEAD
			/*ROS*/
			traf.color = GREEN;
			/**/


=======
>>>>>>> 6660b23fa2a0885e55cae07488180d36cbcd4016
			hasDetectedRedHue = false;
			prevCenter = cv::Point(-1, -1);
		}

		cv::imshow("Video Captured", segmentedInputFrame_BGR);
		cv::imshow("Red Hue", redHueImg);
	}

	return 0;
}
