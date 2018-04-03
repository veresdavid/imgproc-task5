#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include "util.h"
#include "threshold.h"
#include "segmentation.h"
#include "feature.h"

using namespace std;
using namespace cv;

void segmentingTrainImage(const Mat& trainImage, vector<Mat>& output, vector<character>& geometricInformation) {

	// clear output
	output.clear();
	geometricInformation.clear();

	// convert train image to binary: only white or black
	Mat binary;
	imageToBinary(trainImage, binary);

	// extract lines
	vector<textLine> lines;
	extractLinesFromImage(binary, lines);

	// extract characters from lines
	vector<character> characters;
	extractCharactersFromLines(binary, lines, characters);

	// cut off blank spaces from the top and bottom of the characters
	cutOffBlankSpacesFromCharacters(binary, characters, geometricInformation);

	// resize characters to 64x64
	charactersToMats(binary, geometricInformation, output, Size(MAT_ROWS, MAT_COLS));

}

void train(const Mat& trainImage, const vector<Mat>& walshMatrices, database& db) {

	// clear database
	db.data.clear();

	// segmenting train image
	vector<Mat> trainCharacters;
	vector<character> geometricInformation;
	segmentingTrainImage(trainImage, trainCharacters, geometricInformation);

	// iterate over train characters
	for (int i = 0; i < trainCharacters.size(); i++) {

		// data structure to store information about current character
		characterFeatures cf;

		// current character
		Mat trainCharacter = trainCharacters[i];

		// display character
		imshow("train", trainCharacter);
		waitKey(1);

		// read from stdin, which character it is
		cin >> cf.c;

		// calculate feature vector
		vector<int> featureVector;
		featureVectorOfMat(trainCharacter, walshMatrices, geometricInformation[i], cf.featureVector);

		// store in database
		db.data.push_back(cf);

	}

}