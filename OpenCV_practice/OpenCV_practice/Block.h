#ifndef BLOCK_HEADER
#define BLOCK_HEADER

#include "opencv2/imgproc/imgproc.hpp"
#include <vector>

using namespace cv;
using namespace std;

struct Block
{
public:
	Block();
	Block(int index, vector<Point2f> points);
	Block(int index, vector<Point2f> points, bool flag);
	
	void setPoints(vector<Point2f>);
	void setIndex(int index);
	void setIsInObject(bool flag);
	void setCenterPoint();
	Point2f getCenterPoint();
	int getIndex();
	vector<Point2f> getPoints();
	bool isHaveObject();

private:
	vector<Point2f> points;
	int index;
	Point2f center_point;
	bool isInObject = false;
};
#endif