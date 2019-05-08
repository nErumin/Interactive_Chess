#include "ImageProcessor.h"

int main() {
	ImageProcessor ip;
	String title = "test2.jpg";
	vector<Block> blocks = ip.findChessboardBlocks(title);

	vector<Block> w_objects = ip.findChessObject(blocks, title, WHITE);
	vector<Block> b_objects = ip.findChessObject(blocks, title, BLACK);

	w_objects.insert(w_objects.end(), b_objects.begin(), b_objects.end());
	Mat image = imread(title);

	for (vector<Block>::iterator iter = w_objects.begin(); iter != w_objects.end(); iter++) {
		for (int i = 0; i < 4; i++) circle(image, Point((*iter).getPoints()[i]), 5, Scalar(255), 1, 8, 0);
	}

	cv::namedWindow("Detected edge point");
	cv::imshow("Detected edge point", image);

	waitKey(0);
	return 0;
}
