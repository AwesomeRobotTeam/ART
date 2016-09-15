/*
 * Calibration and segment through 4 corners, and transform into 512*512
 *
 * Reference: https://github.com/yati-sagade/opencv-ndarray-conversion
 *
 * By Guan-Wen Lin
 * Last modified: Sep 14, 2016.
 */

#include <iostream>

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

	cv::Point topLeftPt, bottomRightPt;
	cv::Mat warp_mat;

	bool calibrate(PyObject *_inputFrame_BGR, int key) {
		NDArrayConverter cvt;
		cv::Mat inputFrame_BGR = cvt.toMat(_inputFrame_BGR);

		cv::Mat inputFrame_gray;
		cv::cvtColor(inputFrame_BGR, inputFrame_gray, CV_BGR2GRAY);

		cv::Mat cornerResponses;
		cv::cornerHarris(inputFrame_gray, cornerResponses, 2, 3, 0.04);

		cv::Mat cornerResponses_norm;
		cv::normalize(cornerResponses, cornerResponses_norm, 0, 255, cv::NORM_MINMAX);

		int cornerCount = 0;
		bool isFailed = false;
		for (int i = 10; i < cornerResponses_norm.rows - 10; i++) {
			if (isFailed)
				break;

			for (int j = 10; j < cornerResponses_norm.cols - 10; j++) {
				if ((int) cornerResponses_norm.at<float>(i, j) > 200) {
					cornerCount++;
					if (cornerCount > 4) {
						isFailed = true;
						break;
					}

					if (j < cornerResponses_norm.cols / 2 && i < cornerResponses_norm.rows / 2)
						p0 = cv::Point(j, i);
					else if (j > cornerResponses_norm.cols / 2 && i < cornerResponses_norm.rows / 2)
						p1 = cv::Point(j, i);
					else if (j > cornerResponses_norm.cols / 2 && i > cornerResponses_norm.rows / 2)
						p2 = cv::Point(j, i);
					else if (j < cornerResponses_norm.cols / 2 && i > cornerResponses_norm.rows / 2)
						p3 = cv::Point(j, i);
				}
			}
		}

		if (cornerCount == 4) {
			cv::circle(inputFrame_BGR, p0, 5, cv::Scalar(0, 0, 255));
			cv::circle(inputFrame_BGR, p1, 5, cv::Scalar(0, 0, 255));
			cv::circle(inputFrame_BGR, p2, 5, cv::Scalar(0, 0, 255));
			cv::circle(inputFrame_BGR, p3, 5, cv::Scalar(0, 0, 255));

			if ((key & 0xFF) == 27) { // 'Esc' key
				topLeftPt.x = p0.x < p3.x ? p0.x : p3.x;
				topLeftPt.y = p0.y < p1.y ? p0.y : p1.y;
				bottomRightPt.x = p2.x > p1.x ? p2.x : p1.x;
				bottomRightPt.y = p2.y > p3.y ? p2.y : p3.y;

				cv::Point2f srcPt[4];
				cv::Point2f dstPt[4];

				srcPt[0] = p0;
				srcPt[1] = p1;
				srcPt[2] = p2;
				srcPt[3] = p3;

				dstPt[0] = topLeftPt;
				dstPt[1] = cv::Point(bottomRightPt.x, topLeftPt.y);
				dstPt[2] = bottomRightPt;
				dstPt[3] = cv::Point(topLeftPt.x, bottomRightPt.y);

				warp_mat = cv::getPerspectiveTransform(srcPt, dstPt);

				return true;
			}
		}

		cv::imshow("Video Captured", inputFrame_BGR);

		return false;
	}

	PyObject *segment(PyObject *_inputFrame) {
		NDArrayConverter cvt;
		cv::Mat inputFrame = cvt.toMat(_inputFrame);

		/// Transform an image surrounded by four points into a rectangular one

		cv::Mat warp_dst = cv::Mat::zeros(inputFrame.rows, inputFrame.cols, inputFrame.type());
		cv::warpPerspective(inputFrame, warp_dst, warp_mat, warp_dst.size());

		cv::Mat patch;
		warp_dst(cv::Rect(topLeftPt.x, topLeftPt.y, bottomRightPt.x - topLeftPt.x, bottomRightPt.y - topLeftPt.y)).copyTo(patch);

		/// Resize a rectangular image to a square one

		cv::Mat patch_resized(cv::Size(512, 512), patch.type());
		cv::resize(patch, patch_resized, cv::Size(512, 512));

		cv::imshow("Video Captured", inputFrame);
		cv::imshow("Patch Resized", patch_resized);

		return cvt.toNDArray(patch_resized);
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
