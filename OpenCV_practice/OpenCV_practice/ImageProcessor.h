#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "Vector2.h"
#include "Observable.h"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

class ImageProcessor : public Observable<Vector2>
{
private:
	vector<Vector2> chessboard_corners;
	vector<Vector2> chess_pieces;
	vector<Point2f> center_positions;
	Point2f mean_distance;
	Point2f calculateMeanDistanceOfCorners(vector<Point2f> corners);
	vector<Point2f> calculateCenterPositionsOfCells(vector<Point2f> corners);

	Mat thresholdImage(Mat image);
	Mat findBiggestBlob(Mat image);
	vector<Point2f> findIntersection(vector<Vec2f> lines);
	vector<Point2f> findEdge(vector<Point2f> points);
	vector<Point2f> calculateCorners(vector<Point2f> edges);
	vector<vector<Point2f>> findBlocks(vector<Point2f> corners);

public:
	ImageProcessor();
	~ImageProcessor() = default;
	bool detectAndDrawChessboardCorners(String img_name);
	void recognizeMovement();
};

#endif