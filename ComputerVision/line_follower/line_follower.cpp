/*
 * Preferred parameters: -v 0 -s 3 -w 200 -h 50 -g 10 -t 20
 *
 * By Guan-Wen Lin
 * Last modified: Sep 20, 2016.
 */

#include <iostream>
#include <unistd.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define GO_STRAIGHT 0
#define TURN_LEFT   1
#define TURN_RIGHT  2

cv::Mat voteMovingDirection(cv::Mat segmentedInputFrame, int &votes_left, int &votes_middle, int &votes_right, cv::Size windowSize, int windowGap) {
	cv::Point imgCenter(segmentedInputFrame.cols / 2, segmentedInputFrame.rows / 2);
	cv::Point windowOrigin_left  (imgCenter.x - 3 * windowSize.width / 2 - windowGap, imgCenter.y - windowSize.height / 2);
	cv::Point windowOrigin_middle(imgCenter.x -     windowSize.width / 2            , imgCenter.y - windowSize.height / 2);
	cv::Point windowOrigin_right (imgCenter.x +     windowSize.width / 2 + windowGap, imgCenter.y - windowSize.height / 2);

	cv::Rect window_left  (windowOrigin_left  , windowSize);
	cv::Rect window_middle(windowOrigin_middle, windowSize);
	cv::Rect window_right (windowOrigin_right , windowSize);

	cv::Point center_left(0, 0), center_middle(0, 0), center_right(0, 0);
	int ptCount_left = 0, ptCount_middle = 0, ptCount_right = 0;

	cv::Mat edges;
	cv::Canny(segmentedInputFrame, edges, 50, 200);

	for (int i = 0; i < edges.cols; i++) {
		for (int j = windowOrigin_left.y; j < windowOrigin_left.y + windowSize.height; j++) {
			if (edges.at<uchar>(j, i) > 0) {
				cv::Point pt(i, j);
				if (window_left.contains(pt)) {
					center_left += pt;
					ptCount_left++;
				}
				else if (window_middle.contains(pt)) {
					center_middle += pt;
					ptCount_middle++;
				}
				else if (window_right.contains(pt)) {
					center_right += pt;
					ptCount_right++;
				}
			}
		}
	}

	center_left   = ptCount_left   > 0 ? cv::Point(center_left.x   / ptCount_left  , center_left.y   / ptCount_left)   : cv::Point(0, 0);
	center_middle = ptCount_middle > 0 ? cv::Point(center_middle.x / ptCount_middle, center_middle.y / ptCount_middle) : cv::Point(0, 0);
	center_right  = ptCount_right  > 0 ? cv::Point(center_right.x  / ptCount_right , center_right.y  / ptCount_right)  : cv::Point(0, 0);

	votes_left   = window_left.contains(center_left)     ? votes_left   + 1 : votes_left   + 0;
	votes_middle = window_middle.contains(center_middle) ? votes_middle + 1 : votes_middle + 0;
	votes_right  = window_right.contains(center_right)   ? votes_right  + 1 : votes_right  + 0;

	cv::rectangle(edges, window_left  , cv::Scalar(255));
	cv::rectangle(edges, window_middle, cv::Scalar(255));
	cv::rectangle(edges, window_right , cv::Scalar(255));

	return edges;
}

int computeMovingDirection(int preMovingDirection, int &votes_left, int &votes_middle, int &votes_right, int votingThreshold) {
	int curMovingDirection;

	if (votes_left > votingThreshold || votes_middle > votingThreshold || votes_right > votingThreshold) {
		if (votes_left > votingThreshold)
			curMovingDirection = TURN_LEFT;
		else if (votes_middle > votingThreshold)
			curMovingDirection = GO_STRAIGHT;
		else if (votes_right > votingThreshold)
			curMovingDirection = TURN_RIGHT;

		votes_left = votes_middle = votes_right = 0;
	}
	else
		curMovingDirection = preMovingDirection;

	return curMovingDirection;
}

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE, SEGMENTATION_RATIO, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_GAP, VOTING_THRESHOLD;
	while ((c = getopt(argc, argv, "v:s:w:h:g:t:")) != -1) {
		switch (c) {
			case 'v':
				DEVICE = atoi(optarg);
				break;
			case 's':
				SEGMENTATION_RATIO = atoi(optarg);
				break;
			case 'w':
				WINDOW_WIDTH = atoi(optarg);
				break;
			case 'h':
				WINDOW_HEIGHT = atoi(optarg);
				break;
			case 'g':
				WINDOW_GAP = atoi(optarg);
				break;
			case 't':
				VOTING_THRESHOLD = atoi(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-w window_width] [-h window_height] [-g window_gap] [-t voting_threshold]" << std::endl;
				return -1;
		}
	}
	if (argc != 13) {
		std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-w window_width] [-h window_height] [-g window_gap] [-t voting_threshold]" << std::endl;
		return -1;
	}

	cv::VideoCapture myVideoCapture(DEVICE);
	if (!myVideoCapture.isOpened()) {
		std::cout << "ERROR: Cannot open device " << DEVICE << std::endl;
		return -1;
	}

	cv::namedWindow("Video Captured");
	cv::namedWindow("Voting Result");

	int votes_left = 0, votes_right = 0, votes_middle = 0;
	int movingDirection = GO_STRAIGHT;

	while (1) {
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 0;

		cv::Mat inputFrame, segmentedInputFrame;
		myVideoCapture >> inputFrame;
		inputFrame(cv::Range(inputFrame.rows - inputFrame.rows / SEGMENTATION_RATIO, inputFrame.rows), cv::Range::all()).copyTo(segmentedInputFrame);

		cv::Mat edges = voteMovingDirection(segmentedInputFrame, votes_left, votes_right, votes_middle, cv::Size(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_GAP);

		movingDirection = computeMovingDirection(movingDirection, votes_left, votes_middle, votes_right, VOTING_THRESHOLD);
		std::cout << movingDirection << std::endl;

		cv::imshow("Video Captured", segmentedInputFrame);
		cv::imshow("Voting Result", edges);
	}

	return 0;
}
