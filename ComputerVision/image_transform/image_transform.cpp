/*
 * Usage: ./image_transform [src_image_dir] [dst_image_dir] [offset_position] [offset_intensity] [gaussian_kernel_size]
 *  1. (x, y) is translated with an amount between 0 and (offset_position - 1).
 *  2. The intensity is shifted with an offset between (-offset_intensity) and (offset_intensity - 1).
 *  3. The image is blurred using a Gaussian filter with the kernel size equal to (gaussian_kernek_size * gaussian_kernel_size). The value 0 indicates no blurring.
 *
 * By Guan-Wen Lin
 * Last modified: Sep 18, 2016.
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>
#include <dirent.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main(int argc, char *argv[]) {
	if (argc != 6) {
		std::cout << "ERROR: Wrong arguments" << std::endl;
		return -1;
	}
	const int OFFSET_POSITION = atoi(argv[3]);
	const int OFFSET_INTENSITY = atoi(argv[4]);
	const int GAUSSIAN_KERNEL_SIZE = atoi(argv[5]);

	DIR *pSrcDir = opendir(argv[1]);
	DIR *pDstDir = opendir(argv[2]);
	if (!pSrcDir) {
		std::cout << "ERROR: Cannot open " << argv[1] << std::endl;
		return -1;
	}
	if (!pDstDir) {
		std::cout << "ERROR: Cannot open " << argv[2] << std::endl;
		return -1;
	}

	srand(time(NULL));

	struct dirent *pEntry = NULL;
	while ((pEntry = readdir(pSrcDir)) != NULL) {
		if (strcmp(pEntry->d_name, ".") == 0 || strcmp(pEntry->d_name, "..") == 0)
			continue;

		// Cascade the path name
		std::stringstream ssSrc, ssDst;
		ssSrc << argv[1] << pEntry->d_name;
		ssDst << argv[2] << pEntry->d_name;

		cv::Point2f srcPt[4];
		cv::Point2f dstPt[4];		

		std::cout << "Processing " << ssSrc.str() << std::endl;

		cv::Mat src = cv::imread(ssSrc.str());
		cv::Mat warp_dst = cv::Mat::zeros(src.rows, src.cols, src.type());


		/// Blur the image

		if (GAUSSIAN_KERNEL_SIZE != 0)
			cv::GaussianBlur(src, src, cv::Size(GAUSSIAN_KERNEL_SIZE, GAUSSIAN_KERNEL_SIZE), 0, 0);


		/// Change the intensity

		if (OFFSET_INTENSITY != 0) {
			// Convert to HSV 
			cv::Mat src_hsv;
			cv::cvtColor(src, src_hsv, CV_BGR2HSV);
			std::vector<cv::Mat> channels;
			cv::split(src_hsv, channels);
			channels[2] = channels[2] + (rand() % (OFFSET_INTENSITY * 2) - OFFSET_INTENSITY);
			cv::merge(channels, src_hsv);

			// Convert to BGR space
			cv::cvtColor(src_hsv, src, CV_HSV2BGR);
		}


		/// Do perspective transform

		srcPt[0] = cv::Point2f(0, 0);                       // top-left
		srcPt[1] = cv::Point2f(src.cols - 1, 0);            // top-right
		srcPt[2] = cv::Point2f(src.cols - 1, src.rows - 1); // bottom-right
		srcPt[3] = cv::Point2f(0, src.rows - 1);            // bottom-left

		if (OFFSET_POSITION != 0) {
			dstPt[0] = cv::Point2f(0 + rand() % OFFSET_POSITION, 0 + rand() % OFFSET_POSITION);                       // top-left
			dstPt[1] = cv::Point2f(src.cols - 1 - rand() % OFFSET_POSITION, 0 + rand() % OFFSET_POSITION);            // top-right
			dstPt[2] = cv::Point2f(src.cols - 1 - rand() % OFFSET_POSITION, src.rows - 1 - rand() % OFFSET_POSITION); // bottom-right
			dstPt[3] = cv::Point2f(0 + rand() % OFFSET_POSITION, src.rows - 1 - rand() % OFFSET_POSITION);            // bottom-left
		}
		else {
			dstPt[0] = srcPt[0];
			dstPt[1] = srcPt[1];
			dstPt[2] = srcPt[2];
			dstPt[3] = srcPt[3];
		}

		cv::Mat warp_mat = cv::getPerspectiveTransform(srcPt, dstPt);

		cv::warpPerspective(src, warp_dst, warp_mat, warp_dst.size());

		cv::imwrite(ssDst.str(), warp_dst);
	}

	closedir(pSrcDir);
	closedir(pDstDir);

	return 0;
}
