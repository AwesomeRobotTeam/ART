/*
 * Preferred parameters: -v 0 -s 3 -w 125 -h 100 -g 5 -r 2 -t 20
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

cv::Mat voteMovingDirection(cv::Mat segmentedInputFrame, int &votes_left, int &votes_middle, int &votes_right, int windowWidth, int windowHeight, int windowGap, int windowRatio) {
	cv::Point imgCenter(segmentedInputFrame.cols / 2, segmentedInputFrame.rows / 2);
	cv::Point windowOrigin_left  (imgCenter.x - (2 * windowRatio + 1) * windowWidth / 2 - windowGap, imgCenter.y - windowHeight / 2);
	cv::Point windowOrigin_middle(imgCenter.x -                         windowWidth / 2            , imgCenter.y - windowHeight / 2);
	cv::Point windowOrigin_right (imgCenter.x +                         windowWidth / 2 + windowGap, imgCenter.y - windowHeight / 2);

	cv::Rect window_left  (windowOrigin_left  , cv::Size(windowWidth * windowRatio, windowHeight));
	cv::Rect window_middle(windowOrigin_middle, cv::Size(windowWidth              , windowHeight));
	cv::Rect window_right (windowOrigin_right , cv::Size(windowWidth * windowRatio, windowHeight));

	cv::Point center_left(0, 0), center_right(0, 0), center_avg(0, 0);
	int ptCount_left = 0, ptCount_right = 0;

	cv::Mat edges;
	cv::Canny(segmentedInputFrame, edges, 50, 200);

	for (int i = 0; i < edges.cols; i++) {
		for (int j = windowOrigin_left.y; j < windowOrigin_left.y + windowHeight; j++) {
			if (edges.at<uchar>(j, i) > 0) {
				cv::Point pt(i, j);
				if (window_left.contains(pt)) {
					center_left += pt;
					ptCount_left++;
				}
				else if (window_right.contains(pt)) {
					center_right += pt;
					ptCount_right++;
				}
			}
		}
	}

	center_left  = ptCount_left  > 0 ? cv::Point(center_left.x  / ptCount_left , center_left.y  / ptCount_left)  : cv::Point(0, 0);
	center_right = ptCount_right > 0 ? cv::Point(center_right.x / ptCount_right, center_right.y / ptCount_right) : cv::Point(0, 0);

	if      (ptCount_left > 0  && ptCount_right > 0)  center_avg = cv::Point((center_left.x + center_right.x) / 2, (center_left.y + center_right.y) / 2);
	else if (ptCount_left == 0 && ptCount_right > 0)  center_avg = center_right;
	else if (ptCount_left > 0  && ptCount_right == 0) center_avg = center_left;
	else                                              center_avg = imgCenter;

	votes_left   = window_left.contains(center_avg)   ? votes_left   + 1 : votes_left   + 0;
	votes_middle = window_middle.contains(center_avg) ? votes_middle + 1 : votes_middle + 0;
	votes_right  = window_right.contains(center_avg)  ? votes_right  + 1 : votes_right  + 0;

	cv::circle(edges, center_avg, 3, cv::Scalar(255));
	cv::rectangle(edges, window_left  , cv::Scalar(255));
	cv::rectangle(edges, window_middle, cv::Scalar(255));
	cv::rectangle(edges, window_right , cv::Scalar(255));

	return edges;
}

int computeMovingDirection(int preMovingDirection, int &votes_left, int &votes_middle, int &votes_right, int votingThreshold) {
	int curMovingDirection;

	if (votes_left > votingThreshold || votes_middle > votingThreshold || votes_right > votingThreshold) {
		if (votes_left > votingThreshold) {
			if (votes_right == 0) curMovingDirection = TURN_RIGHT;
			else                  curMovingDirection = TURN_LEFT;
		}
		else if (votes_middle > votingThreshold)
			curMovingDirection = GO_STRAIGHT;
		else if (votes_right > votingThreshold) {
			if (votes_left == 0) curMovingDirection = TURN_LEFT;
			else                 curMovingDirection = TURN_RIGHT;
		}

		votes_left = votes_middle = votes_right = 0;
	}
	else
		curMovingDirection = preMovingDirection;

	return curMovingDirection;
}

int main(int argc, char *argv[]) {
	// Check command arguments
	int c;
	int DEVICE, SEGMENTATION_RATIO, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_GAP, WINDOW_RATIO, VOTING_THRESHOLD;
	while ((c = getopt(argc, argv, "v:s:w:h:g:r:t:")) != -1) {
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
			case 'r':
				WINDOW_RATIO = atoi(optarg);
				break;
			case 't':
				VOTING_THRESHOLD = atoi(optarg);
				break;
			default: // '?'
				std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-w window_width] [-h window_height] [-g window_gap] [-r window_ratio] [-t voting_threshold]" << std::endl;
				return -1;
		}
	}
	if (argc != 15) {
		std::cout << "Usage: " << argv[0] << " [-v device] [-s segmentation_ratio] [-w window_width] [-h window_height] [-g window_gap] [-r window_ratio] [-t voting_threshold]" << std::endl;
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

		cv::Mat edges = voteMovingDirection(segmentedInputFrame, votes_left, votes_middle, votes_right, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_GAP, WINDOW_RATIO);

		movingDirection = computeMovingDirection(movingDirection, votes_left, votes_middle, votes_right, VOTING_THRESHOLD);
		std::cout << movingDirection << std::endl;

		cv::imshow("Video Captured", segmentedInputFrame);
		cv::imshow("Voting Result", edges);
	}

	return 0;
}
