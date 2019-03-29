#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

using namespace cv;
using namespace std;

class ImageProcessor {
private:
	Mat img;

public:
	ImageProcessor();
	~ImageProcessor() = default;
	bool detectAndDrawChessboardCorners(String img_name);
};
#endif