#ifndef __RECORD_VIDEO_H__
#define __RECORD_VIDEO_H__

#include <string>
#include <ctime>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int recordVideo(void) {
	cv::VideoCapture myVideoCapture(0);
	if (!myVideoCapture.isOpened())
		return -1;

	time_t t = time(0); // get time now
	struct tm *now = localtime(&t);
	std::string tstr = "VID_" + std::to_string((now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday) + "_" + std::to_string(now->tm_hour * 10000 + now->tm_min * 100 + now->tm_sec) + ".avi";
	cv::VideoWriter myVideoWriter(tstr, CV_FOURCC('M', 'J', 'P', 'G'), 25.0, cv::Size(myVideoCapture.get(CV_CAP_PROP_FRAME_WIDTH), myVideoCapture.get(CV_CAP_PROP_FRAME_HEIGHT)), true);
	if (!myVideoWriter.isOpened())
		return -1;

	cv::namedWindow("Video Captured");

	while (1) {
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27) // 'Esc' key
			return 1;

		cv::Mat inputFrame;
		myVideoCapture >> inputFrame;
		myVideoWriter << inputFrame;
		cv::imshow("Video Captured", inputFrame);
	}
}

#endif
