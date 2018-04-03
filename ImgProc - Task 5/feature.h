#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "util.h"
#include "structs.h"

using namespace std;
using namespace cv;

int calculateFeatureNumber(const Mat& character, const Mat& walshMatrix) {

	int result = 0;

	// logical and operation with Mats, and count the black pixels
	for (int i = 0; i < MAT_ROWS; i++) {
		for (int j = 0; j < MAT_COLS; j++) {

			// get colors
			Vec3b colorCharacter = character.at<Vec3b>(i, j);
			Vec3b colorWalshMatrix = walshMatrix.at<Vec3b>(i, j);

			// logical and
			if (colorCharacter == WHITE && colorWalshMatrix == WHITE) {
				result++;
			}

		}
	}

	return result;

}

void featureVectorOfMat(const Mat& characterMat, const vector<Mat>& walshMatrices, const character& geometric, vector<int>& output) {

	// clear output
	output.clear();

	// calculate feature vector
	for (int i = 0; i < walshMatrices.size(); i++) {

		int feature = calculateFeatureNumber(characterMat, walshMatrices[i]);

		output.push_back(feature);

	}

	// extra features

	// width
	int width = geometric.columnEnd - geometric.columnBegin;
	output.push_back(width * width);

	// height
	int height = geometric.lineEnd - geometric.lineBegin;
	output.push_back(height * height);

}