/*
 * 1. Calibration and segment through 4 corners, and transform into 512*512
 * 2. Output and extract calibration data through a file called calibration_data.out
 *
 * Reference: https://github.com/yati-sagade/opencv-ndarray-conversion
 *
 * By Guan-Wen Lin
 * Last modified: Sep 15, 2016.
 */

#include <iostream>
#include <fstream>

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

	void init(char *fileName) {
		std::fstream file;
		file.open(fileName, std::ios::in);

		file >> p0.x >> p0.y;
		file >> p1.x >> p1.y;
		file >> p2.x >> p2.y;
		file >> p3.x >> p3.y;

		file >> topLeftPt.x >> topLeftPt.y;
		file >> bottomRightPt.x >> bottomRightPt.y;

		warp_mat = cv::Mat(3, 3, CV_64FC1);
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++)
				file >> warp_mat.at<double>(i, j);
		}

		file.close();
	}

	std::string calibrate(PyObject *_inputFrame_BGR, int blockSize, int thresh, int key) {
		NDArrayConverter cvt;
		cv::Mat inputFrame_BGR = cvt.toMat(_inputFrame_BGR);

		cv::Mat inputFrame_gray;
		cv::cvtColor(inputFrame_BGR, inputFrame_gray, CV_BGR2GRAY);

		cv::Mat cornerResponses;
		cv::cornerHarris(inputFrame_gray, cornerResponses, blockSize, 3, 0.04);

		cv::Mat cornerResponses_norm;
		cv::normalize(cornerResponses, cornerResponses_norm, 0, 255, cv::NORM_MINMAX);

		int cornerCount = 0;
		bool isFailed = false;
		for (int i = 10; i < cornerResponses_norm.rows - 10; i++) {
			if (isFailed)
				break;

			for (int j = 10; j < cornerResponses_norm.cols - 10; j++) {
				if ((int) cornerResponses_norm.at<float>(i, j) > thresh) {
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

				/// Output calibration data to a file

				std::string fileName = "calibration_data.out";

				std::fstream file;
				file.open(fileName.c_str(), std::ios::out);

				for (int i = 0; i < 4; i++)
					file << srcPt[i].x << " " << srcPt[i].y << std::endl;
				for (int i = 0; i < 4; i += 2)
					file << dstPt[i].x << " " << dstPt[i].y << std::endl;
				for (int i = 0; i < 3; i++) {
					for (int j = 0; j < 3; j++)
						file << warp_mat.at<double>(i, j) << " ";
					file << std::endl;
				}

				file.close();

				return fileName;
			}
		}

		cv::imshow("Video Captured", inputFrame_BGR);

		return "";
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
		.def("init", &Fetcher::init)
		.def("calibrate", &Fetcher::calibrate)
		.def("segment", &Fetcher::segment)
		;
}
