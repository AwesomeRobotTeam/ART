/*
 * By Guan-Wen Lin
 * Last modified: Sep 14, 2016.
 */

#include <iostream>
#include <vector>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE, PROPORTION;
	while ((c = getopt(argc, argv, "v:p:")) != -1) {
		switch (c) {
			case 'v':
				DEVICE = atoi(optarg);
				break;
			case 'p':
				PROPORTION = atoi(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device] [-p proportion]" << std::endl;
				return -1;
		}
	}
	if (argc != 5) {
		std::cout << "Usage: " << argv[0] << " [-v device] [-p proportion]" << std::endl;
		return -1;
	}

	cv::VideoCapture myVideoCapture(DEVICE);
	if (!myVideoCapture.isOpened()) {
		std::cout << "ERROR: Cannot open device " << DEVICE << std::endl;
		return -1;
	}

	cv::namedWindow("Video Captured");
	cv::namedWindow("Edges Detected");

	while (1) {
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 0;

		cv::Mat inputFrame, inputFrame_segmented;
		myVideoCapture >> inputFrame;
		inputFrame(cv::Range(inputFrame.rows - inputFrame.rows / PROPORTION, inputFrame.rows - 1), cv::Range::all()).copyTo(inputFrame_segmented);

		cv::Mat detectedEdges;
		cv::Canny(inputFrame_segmented, detectedEdges, 50, 200, 3);

		cv::Point center(0, 0);
		int whitePtCount = 0;
		for (int i = 0; i < detectedEdges.rows; i++) {
			for (int j = 0; j < detectedEdges.cols; j++) {
				if (detectedEdges.at<uchar>(i, j) == 255) {
					center += cv::Point(j, i);
					whitePtCount++;
				}
			}
		}
		if (whitePtCount > 0) {
			center = cv::Point(center.x / whitePtCount, center.y / whitePtCount);
			std::cout << center << std::endl;
		}

		cv::imshow("Video Captured", inputFrame_segmented);
		cv::imshow("Edges Detected", detectedEdges);
	}

	return 0;
}
