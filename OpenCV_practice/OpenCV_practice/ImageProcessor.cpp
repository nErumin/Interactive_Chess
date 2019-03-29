#include "ImageProcessor.h"
ImageProcessor::ImageProcessor(){}

bool ImageProcessor::detectAndDrawChessboardCorners(String img_name)
{
	ImageProcessor::img = imread("D:/Documents/2019_1_cau/Capstone2019/Interactive_Chess/OpenCV_practice/OpenCV_practice/wood_chess_board.jpg");
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
		float distances[6][6][2] = { 0, };
		for (int i = 0, k = 0; i < 6; i++, k += 1) {
			for (int j = 0; j < 6; j++) {
				distances[i][j][0] = corners[(6 * i) + j + k].x - corners[(6 * i) + j + k + 1].x;
				distances[i][j][1] = corners[(6 * i) + j].y - corners[(6 * i) + j + 7].y;
			}
		}

		float mean_distance_x = 0;
		float mean_distance_y = 0;

		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 6; j++) {
				mean_distance_x += distances[i][j][0];
				mean_distance_y += distances[i][j][1];
			}
		}
		mean_distance_x /= 36;
		mean_distance_y /= 36;

		drawChessboardCorners(img, patternsize, Mat(corners), patternfound);

		imshow("result", img);
		moveWindow("result", img.cols / 2, 100);
		waitKey(0);
		return true;
	}
	else {
		return false;
	}
}