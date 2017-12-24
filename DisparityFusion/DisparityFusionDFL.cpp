
#include <iostream>
#include <algorithm>
#include "cmath"
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>


using namespace cv;
using namespace std;


int getMax(Mat image, int jInput) {
	int height = image.rows;
	int width = image.cols;

	int maxValue = -1;
	for (int i = 0; i < height; i++) {
		int value = image.at<uchar>(i, jInput);
		if (maxValue < value) {
			maxValue = value;
		}
	}

	return maxValue;
}

// ------ for single image
void main() {
	Mat small = imread("mccnn.png", 0);
	Mat large = imread("monodepth.png", 0);

	int height_small = small.rows;
	int width_smal   = small.cols;

	int padding_up = 245;
	int padding_bt = 140;
	int padding_lf = 245;
	int padding_rt = 245;
	
	Mat result = large.clone();
	small.copyTo(result(Rect(padding_lf, padding_up, width_smal, height_small)));
	imwrite("detect_copy.png", result);

	int width  = result.cols;
	int height = result.rows;

	// for under part
	int i = height - padding_bt;	
	for (int j = padding_lf; j < width - padding_rt; j++) {
		int fixedValue = getMax(small, j - padding_lf);
		int changeValue = result.at<uchar>(i, j);

		int difference = fixedValue - changeValue;
		for (int m = i; m < height; m++) {
			result.at<uchar>(m, j) = min(255, result.at<uchar>(m, j) + difference);
		}
	}
	//imwrite("case1_fusion1.png", result);

	// for upper part 
	i = padding_up - 1;
	for (int j = padding_lf; j < width - padding_rt; j++) {
		int fixedValue = result.at<uchar>(i + 1, j);
		int changeValue = result.at<uchar>(i, j);

		int difference = fixedValue - changeValue;
		for (int m = i; m >= 0; m--) {
			result.at<uchar>(m, j) = max(0, result.at<uchar>(m, j) + difference);
		}
	}
	//imwrite("fusion2.png", result);

	// for left
	int j = padding_lf - 1;
	for (int i = 0; i < height; i++) {
		int fixedValue = result.at<uchar>(i, j + 1);
		int changeValue = result.at<uchar>(i, j);

		int difference = fixedValue - changeValue;
		for (int m = j; m >= 0; m--) {
			result.at<uchar>(i, m) = min(255, max(0, result.at<uchar>(i, m) + difference));
		}
	}
	//imwrite("fusion3.png", result);

	// for right
	j = width - padding_rt - 1;
	for (int i = 0; i < height; i++) {
		int fixedValue = result.at<uchar>(i, j);
		int changeValue = result.at<uchar>(i, j + 1);

		int difference = fixedValue - changeValue;
		for (int m = j + 1; m < width; m++) {
			result.at<uchar>(i, m) = min(255, max(0, result.at<uchar>(i, m) + difference));
		}
	}

	imshow("result", result);
	imwrite("detect_Fusion.png", result);
	waitKey(0);

	//filterMap_Gaussian("detect_fusion.png", "detect_Fusion_Filter.png", 120);
}
