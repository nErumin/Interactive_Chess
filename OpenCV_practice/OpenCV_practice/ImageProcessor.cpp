#include "ImageProcessor.h"
#include <chrono>
#include <thread>

ImageProcessor::ImageProcessor(){
}

bool ImageProcessor::detectAndDrawChessboardCorners(String img_name)
{
	cout << "h3" << endl;
	String filePath = img_name;
	Mat img = imread(filePath);
	imshow("image", img);
	//moveWindow("image", 40, 40);

	Size patternsize(7, 7); //interior number of corners
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);//source image
	vector<Point2f> corners; //this will be filled by the detected corners
	
	//CALIB_CB_FAST_CHECK saves a lot of time on images
	//that do not contain any chessboard corners
	bool patternfound = findChessboardCorners(gray, patternsize, corners,
		CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
		+ CALIB_CB_FAST_CHECK);
	if (patternfound) {
		
		ImageProcessor::mean_distance = calculateMeanDistanceOfCorners(corners);

		drawChessboardCorners(img, patternsize, Mat(corners), patternfound);

		imshow("result", img);
		moveWindow("result", img.cols / 2, 100);
		waitKey(0);

		destroyWindow("image"); //destroy the created window
		destroyWindow("result"); //destroy the created window

		return true;
	}
	else {
		return false;
	}
}

Point2f ImageProcessor::calculateMeanDistanceOfCorners(vector<Point2f> corners) {
	Point2f mean_distance = Point2f(0, 0);
	Point2f distances[6][6];

	for (int i = 0, k = 0; i < 6; i++, k += 1) {
		for (int j = 0; j < 6; j++) {
			distances[i][j] = Point2f(corners[(6 * i) + j + k].x - corners[(6 * i) + j + k + 1].x, 
									corners[(6 * i) + j].y - corners[(6 * i) + j + 7].y);
		}
	}

	float mean_distance_x = 0;
	float mean_distance_y = 0;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			mean_distance += distances[i][j];
		}
	}
	return mean_distance / 36;
}

vector<Point2f> ImageProcessor::calculateCenterPositionsOfCells(vector<Point2f> corners) {
	vector<Point2f> calculate_positions;
	//to do
	return calculate_positions;
}