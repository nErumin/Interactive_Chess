#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "Vector2.h"
#include <vector>
#include <iostream>
#include "Directory.h"

using namespace cv;
using namespace std;

class ImageProcessor
{
private:
	vector<Vector2> chessboard_corners;
	vector<Vector2> chess_pieces;
	vector<Point2f> center_positions;
	Point2f mean_distance;
	Point2f calculateMeanDistanceOfCorners(vector<Point2f> corners);
	vector<Point2f> calculateCenterPositionsOfCells(vector<Point2f> corners);

public:
	ImageProcessor();
	~ImageProcessor() = default;
	bool detectAndDrawChessboardCorners(String img_name);
};
#endif