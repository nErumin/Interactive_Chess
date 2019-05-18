#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "Vector2.h"
#include "Observable.h"
#include "Block.h"
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

#define TEST 1
#define PI 3.1415926

#define WHITE 0
#define BLACK 1

class ImageProcessor : public Observable<Vector2>
{
private:
	vector<Block> blocks;
	vector<Block> pieces;
	int average_black[3], average_white[3];

	Mat thresholdImage(Mat image);
	Mat findBiggestBlob(Mat image);
	vector<Point2f> findIntersection(vector<Vec2f> lines, int max_x, int max_y);
	vector<Point2f> findEdge(vector<Point2f> points);
	vector<Point2f> calculateCorners(vector<Point2f> edges);
	vector<Block> findBlocks(vector<Point2f> corners);
	vector<Block> findColorObject(String title, int color);
	vector<Block> findChessboardBlocks(String title);
	vector<Block> findChessObject(String title);
	vector<int> comparePieces(vector<Block> newPieces);
	void setAverageColor(String title);
public:
	ImageProcessor();
	~ImageProcessor() = default;
	bool isFirst();
	void initialize(String title);
	void recognizeMovement(String title);
};
#endif