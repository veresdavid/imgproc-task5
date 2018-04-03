#pragma once

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

// dimensions for character and walsh matrices
const int MAT_ROWS = 64;
const int MAT_COLS = 64;

// walsh matrix row or column unit
const int WALSH_ROW_COL_UNIT = 8;

// color vectors
const Vec3b BLACK(0, 0, 0);
const Vec3b WHITE(255, 255, 255);

// number of features stored in database rows
const int NUMBER_OF_FEATURES = 64 + 2;