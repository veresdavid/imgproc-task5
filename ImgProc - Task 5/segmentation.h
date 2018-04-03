#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>
#include "util.h"
#include "threshold.h"
#include "structs.h"

using namespace cv;
using namespace std;

const int BLANK_SECTION = 0;
const int TEXT_SECTION = 1;

void extractLinesFromImage(const Mat& src, vector<textLine>& lines) {

	// source image properties
	int width = src.cols;
	int height = src.rows;

	// initial steps

	// clear output vector
	lines.clear();

	// what section we are currently at?
	int sectionType = BLANK_SECTION;

	// stor the begin index of a text section
	int begin = -1;

	// scan only the first line!
	for (int i = 0; i < width; i++) {

		Vec3b color = src.at<Vec3b>(0, i);

		if (color == BLACK) {
			sectionType = TEXT_SECTION;
			begin = 0;
			break;
		}

	}

	// process the following lines
	for (int i = 1; i < height; i++) {

		// check if the current line is a blank line
		bool isBlankLine = true;

		for (int j = 0; j < width; j++) {

			Vec3b color = src.at<Vec3b>(i, j);

			if (color == BLACK) {
				isBlankLine = false;
				break;
			}

		}

		// process
		if (isBlankLine && sectionType == TEXT_SECTION) {

			textLine l{ begin, i - 1 };
			lines.push_back(l);
			begin = -1;
			sectionType = BLANK_SECTION;

		}
		else if (!isBlankLine && sectionType == BLANK_SECTION) {

			begin = i;
			sectionType = TEXT_SECTION;

		}

	}

	// process last section
	if (sectionType == TEXT_SECTION) {

		textLine l{ begin, height - 1 };
		lines.push_back(l);

	}

}

void extractCharactersFromLines(const Mat& src, const vector<textLine>& lines, vector<character>& characters) {

	// image properties
	int width = src.cols;

	// clear output vector
	characters.clear();

	// iterate over lines
	for (int i = 0; i < lines.size(); i++) {

		// the current line
		textLine l = lines[i];

		// begin column index
		int begin = -1;

		// what section we are currently at?
		int sectionType = BLANK_SECTION;

		// scan only the first column!
		for (int j = l.begin; j <= l.end; j++) {

			Vec3b color = src.at<Vec3b>(j, 0);

			if (color == BLACK) {
				sectionType = TEXT_SECTION;
				break;
			}

		}

		// process the following columns
		for (int j = 1; j < width; j++) {

			// check if the current column is blank
			bool isBlankColumn = true;

			for (int k = l.begin; k <= l.end; k++) {

				Vec3b color = src.at<Vec3b>(k, j);

				if (color == BLACK) {
					isBlankColumn = false;
					break;
				}

			}

			// process
			if (isBlankColumn && sectionType == TEXT_SECTION) {

				character c{ l.begin, l.end, begin, j - 1 };
				characters.push_back(c);
				begin = -1;
				sectionType = BLANK_SECTION;

			}
			else if (!isBlankColumn && sectionType == BLANK_SECTION) {

				begin = j;
				sectionType = TEXT_SECTION;

			}

		}

		// process last section
		if (sectionType == TEXT_SECTION) {

			character c{ l.begin, l.end, begin, width - 1 };
			characters.push_back(c);

		}

	}

}

void cutOffBlankSpacesFromCharacters(const Mat& src, const vector<character>& characters, vector<character>& output) {

	// clear output vector
	output.clear();

	// iterate over characters
	for (int i = 0; i < characters.size(); i++) {

		character c = characters[i];

		int lineBegin = c.lineBegin;
		int lineEnd = c.lineEnd;

		bool foundFirstLine = false;
		bool foundLastLine = false;

		// from top
		for (int j = c.lineBegin; j <= c.lineEnd; j++) {

			for (int k = c.columnBegin; k <= c.columnEnd; k++) {

				Vec3b color = src.at<Vec3b>(j, k);

				if (color == BLACK) {
					lineBegin = j;
					foundFirstLine = true;
					break;
				}

			}

			if (foundFirstLine) {
				break;
			}

		}

		// from bottom
		for (int j = c.lineEnd; j >= c.lineBegin; j--) {

			for (int k = c.columnBegin; k <= c.columnEnd; k++) {

				Vec3b color = src.at<Vec3b>(j, k);

				if (color == BLACK) {
					lineEnd = j;
					foundLastLine = true;
					break;
				}

			}

			if (foundLastLine) {
				break;
			}

		}

		// save clean character
		character cleanCharacter{ lineBegin, lineEnd, c.columnBegin, c.columnEnd };
		output.push_back(cleanCharacter);

	}

}

void charactersToMats(const Mat& src, const vector<character>& characters, vector<Mat>& mats, Size size = Size(32, 32)) {

	// clear output vector
	mats.clear();

	// save characters as scaled Mat objects
	for (int i = 0; i < characters.size(); i++) {

		character c = characters[i];

		// create Mat from roi
		Mat cleanMat = src(Rect(Point(c.columnBegin, c.lineBegin), Point(c.columnEnd + 1, c.lineEnd + 1)));

		// resize Mat
		Mat resized;
		resize(cleanMat, resized, size, INTER_LANCZOS4);

		// convert to binary
		Mat binary;
		imageToBinary(resized, binary);

		// save it
		mats.push_back(binary);

	}

}

void saveCharacters(const vector<Mat>& characters, const string& folder) {

	for (int i = 0; i < characters.size(); i++) {

		stringstream path;
		path << folder << "\\" << "char_" << i << ".png";

		imwrite(path.str(), characters[i]);

	}

}