#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "structs.h"
#include "util.h"

using namespace std;

void writeDatabaseToFile(const database& db, const string& path) {
	
	// open file
	ofstream out(path);

	// number of data rows
	out << db.data.size() << endl;

	// write data rows
	for (int i = 0; i < db.data.size(); i++) {

		// current data row
		characterFeatures cf = db.data[i];

		// character
		out << cf.c << " ";

		// feature vector
		for (int j = 0; j < cf.featureVector.size(); j++) {

			out << cf.featureVector[j];

			if (j != cf.featureVector.size() - 1) {
				out << " ";
			}

		}

		// end of line
		if (i != db.data.size() - 1) {
			out << endl;
		}

	}

	// close file
	out.close();

}

void readDatabaseFromFile(const string& path, database& db) {
	
	// clear database
	db.data.clear();

	// open file
	ifstream in(path);

	// read file

	// number of data rows stored in the database
	int numberOfDataRows;
	in >> numberOfDataRows;

	// read data rows
	for (int i = 0; i < numberOfDataRows; i++) {

		characterFeatures cf;
		cf.featureVector.clear();

		// read character
		in >> cf.c;

		// read features
		int feature;
		for (int j = 0; j < NUMBER_OF_FEATURES; j++) {

			in >> feature;

			cf.featureVector.push_back(feature);

		}

		db.data.push_back(cf);

	}

	// close file
	in.close();

}