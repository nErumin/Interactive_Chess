#include "ImageProcessor.h"
#include <chrono>
#include <thread>

ImageProcessor::ImageProcessor() {
	
}

Mat ImageProcessor::thresholdImage(Mat image) {
	Mat input_gray_image = imread("test.jpg", IMREAD_GRAYSCALE);

	namedWindow("입력 이미지", WINDOW_AUTOSIZE);
	namedWindow("otsu 이미지", WINDOW_AUTOSIZE);
	namedWindow("adaptive 이미지", WINDOW_AUTOSIZE);

	Mat result_otsu_image;
	Mat result_adaptive_image;
	//이진화를 한다.

	threshold(input_gray_image, result_otsu_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
	adaptiveThreshold(input_gray_image, result_adaptive_image, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 10);

	imshow("이진화된 이미지", result_otsu_image);
	imshow("otsu 이미지", result_adaptive_image);
	imshow("adaptive 이미지", input_gray_image);

	//아무키나 누를 때 까지 대기한다.
	waitKey(0);
	return result_otsu_image;
}



void ImageProcessor::recognizeMovement() {
	// to do
	notifyToObservers(Vector2(1.0, 2.0));
}

bool ImageProcessor::detectAndDrawChessboardCorners(String img_name)
{
	Mat img = imread(img_name);
	imshow("image", img);
	//moveWindow("image", 40, 40);

	Size patternsize(7, 7); //interior number of corners
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);//source image
	vector<Point2f> corners; //this will be filled by the detected corners
	
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