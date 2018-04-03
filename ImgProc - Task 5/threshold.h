#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "util.h"

using namespace cv;

unsigned char relativeLuminance(const Vec3b& color) {

	double b = color[0];
	double g = color[1];
	double r = color[2];

	double lum = r * 0.2126 + g * 0.7152 + b * 0.0722;

	return round(lum);

}

void imageToBinary(const Mat& src, Mat& dst) {

	int width = src.cols;
	int height = src.rows;

	dst = src.clone();

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			Vec3b color = src.at<Vec3b>(i, j);

			unsigned char lum = relativeLuminance(color);

			if (lum < 128) {
				dst.at<Vec3b>(i, j) = BLACK;
			}
			else {
				dst.at<Vec3b>(i, j) = WHITE;
			}

		}
	}

}