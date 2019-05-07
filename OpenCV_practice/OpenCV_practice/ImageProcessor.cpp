#include "ImageProcessor.h"
#include <chrono>
#include <thread>

ImageProcessor::ImageProcessor() {
	
}

Mat ImageProcessor::thresholdImage(Mat image) {
	Mat input_gray_image = imread("test.jpg", IMREAD_GRAYSCALE);

	namedWindow("입력 이미지", WINDOW_AUTOSIZE);
	namedWindow("otsu 이미지", WINDOW_AUTOSIZE);
	
	Mat result_otsu_image;
	//이진화를 한다.
	threshold(input_gray_image, result_otsu_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("Gray 이미지", input_gray_image);
	imshow("OTSU 이미지", result_otsu_image);

	//아무키나 누를 때 까지 대기한다.
	waitKey(0);
	return result_otsu_image;
}

Mat ImageProcessor::findBiggestBlob(Mat image) {
	int largest_area = 0;
	int largest_contour_index = 0;
	
	vector< vector<Point> > contours; // Vector for storing contour
	vector<Vec4i> hierarchy;

	findContours(image, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE); // Find the contours in the image

	for (int i = 0; i < contours.size(); i++) {// iterate through each contour. 
		double a = contourArea(contours[i], false);  //  Find the area of contour
		if (a > largest_area) {
			largest_area = a;
			largest_contour_index = i;                //Store the index of largest contour
			//bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
		}
	}

	Mat contoursImage = Mat::zeros(image.size(), CV_8UC3);;
	drawContours(contoursImage, contours, largest_contour_index, Scalar(255), FILLED, 8, hierarchy); // Draw the largest contour using previously stored index.
	return contoursImage;
}

vector<Point2f> findIntersection(vector<Vec2f> lines) {
	vector<Point2f> points;

	vector<Vec2f> lines1, lines2;
	lines1.push_back(lines[0]);
	for (vector<Vec2f>::iterator i = lines.begin() + 1; i != lines.end(); i++) {
		if (abs(lines1[0][1] - (*i)[1]) < 0.1) lines1.push_back((*i));
		else lines2.push_back((*i));
	}

	for (vector<Vec2f>::iterator i = lines1.begin(); i != lines1.end(); i++) {
		for (vector<Vec2f>::iterator j = lines2.begin(); j != lines2.end(); j++) {
			Point2f point;
			double a = cos((*i)[1]), b = sin((*i)[1]), c = (*i)[0];
			double m = cos((*j)[1]), n = sin((*j)[1]), p = (*j)[0];
			point.x = (p * b - n * c) / (m * b - n * a);
			point.y = (p * a - m * c) / (n * a - m * b);
			points.push_back(point);
			std::cout << "point: (" << point.x << "," << point.y << ")\n";
		}
	}
	return points;
}

vector<Point2f> findEdge(vector<Point2f> points) {
	vector<Point2f> edges;
	Point point[4];
	for (int i = 0; i < 4; i++) point[i] = points[0];
	for (vector<Point2f>::iterator i = points.begin() + 1; i != points.end(); i++) {
		float x = (*i).x, y = (*i).y;
		float check = x + y;
		if (point[0].x + point[0].y > x + y) {
			point[0] = *i;
		}
		if (point[1].x * (-1) + point[1].y > x * (-1) + y) {
			point[1] = *i;
		}
		if (point[2].x + point[2].y * (-1) > x + y * (-1)) {
			point[2] = *i;
		}
		if (point[3].x * (-1) + point[3].y * (-1) > x * (-1) + y * (-1)) {
			point[3] = *i;
		}
	}
	for (int i = 0; i < 4; i++) edges.push_back(point[i]);
	return edges;
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