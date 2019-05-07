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
	
	void setPoints(vector<Point2f>) noexcept;
	void setIndex(int index) noexcept;
	void setIsInObject(bool flag) noexcept;

	int getIndex() noexcept;
	vector<Point2f> getPoints() noexcept;
	bool haveObject() noexcept;

private:
	vector<Point2f> points;
	int index;
	bool isInObject;
};

#endif