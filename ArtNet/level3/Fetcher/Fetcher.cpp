/*
 * Reference: https://solarianprogrammer.com/2015/05/08/detect-red-circles-image-using-opencv/
 *            https://github.com/yati-sagade/opencv-ndarray-conversion
 *
 * By Guan-Wen Lin
 * Last modified: Aug 6, 2016.
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <boost/python.hpp>

#include "opencv-ndarray-conversion/conversion.h"

struct Fetcher {
	cv::Point p0; // Top-left point
	cv::Point p1; // Top-right point
	cv::Point p2; // Bottom-right point
	cv::Point p3; // Bottom-left point

	bool calibrate(PyObject *_inputFrame_BGR, int key) {
		NDArrayConverter cvt;
		cv::Mat inputFrame_BGR = cvt.toMat(_inputFrame_BGR);

		// Convert input image to HSV
		cv::Mat inputFrame_HSV;
//		cv::GaussianBlur(inputFrame_BGR, inputFrame_BGR, cv::Size(5, 5), 0, 0);
//		cv::resize(inputFrame_BGR, inputFrame_BGR, cv::Size(inputFrame_BGR.cols / 2, inputFrame_BGR.rows / 2));
		cv::cvtColor(inputFrame_BGR, inputFrame_HSV, CV_BGR2HSV);

		// Threshold the HSV image, keep only the red pixels
		cv::Mat lowerRedHueImg;
		cv::Mat upperRedHueImg;
		cv::Mat redHueImg;
		cv::inRange(inputFrame_HSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), lowerRedHueImg);
		cv::inRange(inputFrame_HSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), upperRedHueImg);
		cv::addWeighted(lowerRedHueImg, 1.0, upperRedHueImg, 1.0, 0.0, redHueImg);

		// Find four centers of red pixels in the combined threshold image
		std::vector< std::vector<cv::Point> > contours;
		cv::findContours(redHueImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		if (contours.size() == 4) {
			for (int i = 0; i < contours.size(); i++) {
				cv::Point center(0, 0);
				for (int j = 0; j < contours.at(i).size(); j++)
					center += contours.at(i).at(j);
				center = cv::Point(center.x / contours.at(i).size(), center.y / contours.at(i).size());
				if (center.x < redHueImg.cols / 2 && center.y < redHueImg.rows / 2)
					p0 = center;
				else if (center.x > redHueImg.cols / 2 && center.y < redHueImg.rows / 2)
					p1 = center;
				else if (center.x > redHueImg.cols / 2 && center.y > redHueImg.rows / 2)
					p2 = center;
				else
					p3 = center;

				cv::drawContours(inputFrame_BGR, contours, i, cv::Scalar(255, 0, 0));
				cv::circle(inputFrame_BGR, center, 3, cv::Scalar(255, 0, 0), -1);
			}

			if (p0 != p1 && p0 != p2 && p0 != p3 && p1 != p2 && p1 != p3 && p2 != p3 && (key & 0xFF)  == 27) // 'Esc' key
				return true;
		}

		cv::imshow("Video Captured", inputFrame_BGR);

		return false;
	}

//	cv::Mat segment(cv::Mat inputFrame) {
	PyObject *segment(PyObject *_inputFrame) {
		NDArrayConverter cvt;
		cv::Mat inputFrame = cvt.toMat(_inputFrame);

        cv::Mat patch;
        inputFrame(cv::Rect(p0.x, p0.y, p1.x - p0.x, p3.y - p0.y)).copyTo(patch);

		cv::imshow("Video Captured", inputFrame);
		cv::imshow("Patch", patch);

		return cvt.toNDArray(patch);
	}
};

static void init() {
	Py_Initialize();
	import_array();
}

BOOST_PYTHON_MODULE(fetcher) {
	init();
	boost::python::class_<Fetcher>("Fetcher")
		.def("calibrate", &Fetcher::calibrate)
		.def("segment", &Fetcher::segment)
	;
}
/*
int main(int argc, char *argv[]) {
	cv::VideoCapture myVideoCapture(0);
	if (!myVideoCapture.isOpened())
		return 0;

	cv::namedWindow("Video Captured");
	cv::namedWindow("Patch");

	struct Fetcher myFetcher;
	cv::Mat inputFrame;

	// Calibrate
	while (1) {
		myVideoCapture >> inputFrame;
		int key = cv::waitKey(1);
		if (myFetcher.calibrate(inputFrame, key))
			break;

		cv::imshow("Video Captured", inputFrame);
	}

	// Fetch and segment
	while (1) {
		int key = cv::waitKey(1);
		if ((key & 0xFF) == 27)
			break;

		myVideoCapture >> inputFrame;
		cv::Mat patch = myFetcher.segment(inputFrame);
		cv::imshow("Video Captured", inputFrame);
		cv::imshow("Patch", patch);
	}

	return 0;
}
*/
