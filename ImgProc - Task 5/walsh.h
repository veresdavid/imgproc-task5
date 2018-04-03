#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include "util.h"

using namespace std;
using namespace cv;

// walsh matrix flags, for black rows and columns
const bool MATRIX_FLAGS[8][8] = {
	{ false, false, false, false, false, false, false, false },
	{ false, true, false, true, false, true, false, true },
	{ false, false, true, true, false, false, true, true },
	{ false, true, true, false, false, true, true, false },
	{ false, false, false, false, true, true, true, true },
	{ false, true, false, true, true, false, true, false },
	{ false, false, true, true, true, true, false, false },
	{ false, true, true, false, true, false, false, true }

};

// generate white matrix
void generateBaseMat(Mat& output) {

	// create new Mat
	output = Mat(MAT_ROWS, MAT_COLS, CV_8UC3);

	// change every pixel to white
	for (int i = 0; i < MAT_ROWS; i++) {
		for (int j = 0; j < MAT_COLS; j++) {

			output.at<Vec3b>(i, j) = WHITE;

		}
	}

}

void generateRowWalshMatrix(Mat& output, const bool flags[8]) {
	
	// create base white Mat
	generateBaseMat(output);

	// draw black rows
	for (int i = 0; i < 8; i++) {

		if (flags[i] == true) {

			int startRow = i * WALSH_ROW_COL_UNIT;

			for (int j = startRow; j < startRow + WALSH_ROW_COL_UNIT; j++) {
				for (int k = 0; k < MAT_COLS; k++) {

					output.at<Vec3b>(j, k) = BLACK;

				}
			}

		}

	}

}

void generateColumnWalshMatrix(Mat& output, const bool flags[8]) {

	// create base white Mat
	generateBaseMat(output);

	// draw black columns
	for (int i = 0; i < 8; i++) {

		if (flags[i] == true) {

			int startColumn = i * WALSH_ROW_COL_UNIT;

			for (int j = startColumn; j < startColumn + WALSH_ROW_COL_UNIT; j++) {
				for (int k = 0; k < MAT_ROWS; k++) {

					output.at<Vec3b>(k, j) = BLACK;

				}
			}

		}

	}

}

void generateFirstWalshRow(vector<Mat>& output) {

	// clear output
	output.clear();

	// create first row of walsh matrices
	Mat tmp;
	for (int i = 0; i < 8; i++) {

		generateColumnWalshMatrix(tmp, MATRIX_FLAGS[i]);

		output.push_back(tmp);

	}

}

void generateFirstWalshColumn(vector<Mat>& output) {

	// clear output
	output.clear();

	// create first column of walsh matrices
	Mat tmp;
	for (int i = 0; i < 8; i++) {

		generateRowWalshMatrix(tmp, MATRIX_FLAGS[i]);

		output.push_back(tmp);

	}

}

void xorMats(const Mat& a, const Mat& b, Mat& output) {

	// generate new blank matrix
	generateBaseMat(output);

	// xor
	for (int i = 0; i < MAT_ROWS; i++) {
		for (int j = 0; j < MAT_COLS; j++) {

			// get colors
			Vec3b colorA = a.at<Vec3b>(i, j);
			Vec3b colorB = b.at<Vec3b>(i, j);

			// xor condition
			if ((colorA == WHITE && colorB == BLACK) || (colorA == BLACK && colorB == WHITE)) {
				output.at<Vec3b>(i, j) = BLACK;
			}

		}
	}

}

// generate 8x8 walsh matrices with row-major order to a vector
void generateWalshMatrices(vector<Mat>& output) {

	// clear output vector
	output.clear();

	// generate first row, except the first blank matrix
	vector<Mat> firstRow;
	generateFirstWalshRow(firstRow);

	// generate first column, except the first blank matrix
	vector<Mat> firstColumn;
	generateFirstWalshColumn(firstColumn);

	// add first row to the output
	for (int i = 0; i < firstRow.size(); i++) {
		output.push_back(firstRow[i]);
	}

	// generate remaining matrices with XOR, and add them to the result
	Mat tmp;
	for (int i = 1; i < firstColumn.size(); i++) {

		// add the actual first column matrix to the output
		output.push_back(firstColumn[i]);

		// XOR row and column matrices
		for (int j = 1; j < firstRow.size(); j++) {

			xorMats(firstColumn[i], firstRow[j], tmp);

			output.push_back(tmp);

		}

	}

}