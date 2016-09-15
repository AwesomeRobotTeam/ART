/*
 * By Guan-Wen Lin
 * Last modified: Sep 15, 2016.
 */

#include <iostream>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

cv::Mat extractBlueHue(cv::Mat inputFrame_BGR) {
	// Convert input image to HSV
	cv::Mat inputFrame_HSV;
	cv::cvtColor(inputFrame_BGR, inputFrame_HSV, CV_BGR2HSV);

	// Threshold the HSV image, keep only the blue pixels
	cv::Mat blueHueImg;
	cv::inRange(inputFrame_HSV, cv::Scalar(110, 100, 100), cv::Scalar(130, 255, 255), blueHueImg);

	return blueHueImg;
}

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE;
	while ((c = getopt(argc, argv, "v:")) != -1) {
		switch (c) {
			case 'v':
				DEVICE = atoi(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device]" << std::endl;
				return -1;
		}
	}
	if (argc != 3) {
		std::cout << "Usage: " << argv[0] << " [-v device]" << std::endl;
		return -1;
	}

	cv::VideoCapture myVideoCapture(DEVICE);
	if (!myVideoCapture.isOpened()) {
		std::cout << "ERROR: Cannot open device " << DEVICE << std::endl;
		return -1;
	}

	cv::namedWindow("Video Captured");
	cv::namedWindow("Blue Hue");

	while (1) {
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 0;

		cv::Mat inputFrame_BGR;
		myVideoCapture >> inputFrame_BGR;
		cv::Mat blueHueImg = extractBlueHue(inputFrame_BGR);

		cv::imshow("Video Captured", inputFrame_BGR);
		cv::imshow("Blue Hue", blueHueImg);
	}

	return 0;
}
