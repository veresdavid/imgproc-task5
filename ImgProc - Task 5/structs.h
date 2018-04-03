#pragma once

#include <vector>

using namespace std;

// structs for segmentation
struct textLine {
	int begin;
	int end;
};

struct character {
	int lineBegin;
	int lineEnd;
	int columnBegin;
	int columnEnd;
};

// struct for training and recognition
struct characterFeatures {
	char c;
	vector<int> featureVector;
};

// struct for train database
struct database {
	vector<characterFeatures> data;
};