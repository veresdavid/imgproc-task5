#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "train.h"
#include "recognition.h"
#include "walsh.h"
#include "fileio.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]) {

	if (argc == 4 && string(argv[1]) == "-t") {

		cout << "TRAINING" << endl;

		// read train image
		Mat trainImage = imread(argv[2]);

		// generate walsh matrices
		vector<Mat> walshMatrices;
		generateWalshMatrices(walshMatrices);

		// train
		database db;
		train(trainImage, walshMatrices, db);

		// write database to file
		writeDatabaseToFile(db, argv[3]);

		cout << "TRAINING DONE!" << endl;

	}
	else if (argc == 5 && string(argv[1]) == "-r") {

		cout << "RECOGNITION" << endl;

		// read image
		Mat image = imread(argv[2]);

		// read database from file
		database db;
		readDatabaseFromFile(argv[3], db);

		// generate walsh matrices
		vector<Mat> walshMatrices;
		generateWalshMatrices(walshMatrices);

		// recognition
		stringstream output;
		recognition(image, db, walshMatrices, output);

		// write output to file
		ofstream out(argv[4]);
		out << output.str();
		out.close();

		cout << "RECOGNITION DONE!" << endl;

	}
	else {

		cout << "Error! Invalid command line arguments!" << endl;
		cout << "Usage:" << endl;
		cout << "Training: ./program -t train_image.jpg train_output.txt" << endl;
		cout << "Recognition: ./program -r input_image.jpg database.txt output.txt" << endl;
		return -1;

	}

	return 0;
}