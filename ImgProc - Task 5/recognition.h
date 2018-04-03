#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include "util.h"
#include "threshold.h"
#include "segmentation.h"

using namespace std;
using namespace cv;

const double RECOGNITION_ERROR_THRESHOLD = 100.0;
const double SPACE_WEIGHT = 1.2;
const int NEW_LINE_DISTANCE = 100;

void segmentingInputImage(const Mat& inputImage, vector<Mat>& characterMats, vector<character>& geometricInformation) {

	// clear output
	characterMats.clear();
	geometricInformation.clear();

	// convert train image to binary: only white or black
	Mat binary;
	imageToBinary(inputImage, binary);

	// extract lines
	vector<textLine> lines;
	extractLinesFromImage(binary, lines);

	// extract characters from lines
	vector<character> characters;
	extractCharactersFromLines(binary, lines, characters);

	// cut off blank spaces from the top and bottom of the characters
	cutOffBlankSpacesFromCharacters(binary, characters, geometricInformation);

	// resize characters to 64x64
	charactersToMats(binary, geometricInformation, characterMats, Size(MAT_ROWS, MAT_COLS));

}

double distanceOfVectors(const vector<int>& a, const vector<int>& b) {

	double dist = 0.0;

	for (int i = 0; i < a.size(); i++) {
		dist += (a[i] - b[i])*(a[i] - b[i]);
	}

	dist = sqrt(dist);

	return dist;

}

int getClosestFeatureVectorIndex(const vector<int>& featureVector, const database& db){

	int index = 0;
	double dist = distanceOfVectors(featureVector, db.data[0].featureVector);

	for (int i = 1; i < db.data.size(); i++) {

		double currentDist = distanceOfVectors(featureVector, db.data[i].featureVector);

		if (currentDist < dist) {
			dist = currentDist;
			index = i;
		}

	}

	return index;

}

double getAverageCharacterDistance(const vector<character>& geometricInformation) {

	double sum = 0.0;
	int count = 0;

	for (int i = 1; i < geometricInformation.size(); i++) {

		character current = geometricInformation[i];
		character last = geometricInformation[i - 1];

		// check if not different line
		if (!(abs(current.columnBegin - last.columnBegin) > NEW_LINE_DISTANCE)) {

			sum += (current.columnBegin - last.columnEnd);
			count++;

		}

	}

	return sum / (count * 1.0);

}

void recognition(const Mat& inputImage, const database& db, const vector<Mat>& walshMatrices, stringstream& output) {
	
	// empty stringstream
	output.str("");

	// segmenting input image
	vector<Mat> characterMats;
	vector<character> geometricInformation;
	segmentingInputImage(inputImage, characterMats, geometricInformation);

	// preprocess
	double averageDistance = getAverageCharacterDistance(geometricInformation);

	// store lastly processed character
	Mat lastCharacter = characterMats[0];
	character lastGeometric = geometricInformation[0];

	// iterate over characters
	for (int i = 0; i < characterMats.size(); i++) {

		// current character information
		Mat characterMat = characterMats[i];
		character geometric = geometricInformation[i];

		// new line or space
		if (abs(geometric.columnBegin-lastGeometric.columnBegin) > NEW_LINE_DISTANCE) {
			output << endl;
		}
		else if ((geometric.columnBegin - lastGeometric.columnEnd) > averageDistance * SPACE_WEIGHT) {
			output << " ";
		}

		// get feature vector
		vector<int> featureVector;
		featureVectorOfMat(characterMat, walshMatrices, geometric, featureVector);

		// get closest feature vector from database
		int closestIndex = getClosestFeatureVectorIndex(featureVector, db);

		// return character if close enough, else ?
		char recognizedCharacter = '?';
		if (distanceOfVectors(featureVector, db.data[closestIndex].featureVector) <= RECOGNITION_ERROR_THRESHOLD) {
			recognizedCharacter = db.data[closestIndex].c;
		}

		// output
		output << recognizedCharacter;

		// update last
		lastCharacter = characterMat;
		lastGeometric = geometric;

	}

}