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

vector<Point2f> ImageProcessor::findIntersection(vector<Vec2f> lines, int max_x, int max_y) {
	vector<Point2f> points;

	vector<Vec2f> lines1, lines2;
	lines1.push_back(lines[0]);

	for (vector<Vec2f>::iterator it = lines.begin(); it != lines.end(); it++) {
		float rho = (*it)[0];   // 첫 번째 요소는 rho 거리
		float theta = (*it)[1]; // 두 번째 요소는 델타 각도
		if (theta < PI / 4. || theta > 3.*PI / 4.) lines1.push_back((*it)); // 수직 행
		else lines2.push_back((*it)); // 수평 행
	}

	for (vector<Vec2f>::iterator i = lines1.begin(); i != lines1.end(); i++) {
		for (vector<Vec2f>::iterator j = lines2.begin(); j != lines2.end(); j++) {
			Point2f point;
			double a = cos((*i)[1]), b = sin((*i)[1]), c = (*i)[0];
			double m = cos((*j)[1]), n = sin((*j)[1]), p = (*j)[0];
			point.x = (p * b - n * c) / (m * b - n * a);
			point.y = (p * a - m * c) / (n * a - m * b);

			if (point.x > 0 && point.y > 0 && point.x < max_x && point.y < max_y) points.push_back(point);
			std::cout << "point: (" << point.x << "," << point.y << ")\n";
		}
	}
	return points;
}

vector<Point2f> ImageProcessor::findEdge(vector<Point2f> points) {
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

vector<Point2f> ImageProcessor::calculateCorners(vector<Point2f> edges) {
	vector<Point2f> corners;

	Point2f d[4];
	int order_a[4] = { 1, 2, 3, 3 };
	int order_b[4] = { 0, 0, 1, 2 };

	for (int i = 0; i < 4; i++) d[i] = (edges[order_a[i]] - edges[order_b[i]]) / 8; // 0, 1

	for (int i = 0; i <= 8; i++) {
		Point2f point[2];
		point[0] = edges[0] + d[1] * i;
		point[1] = edges[1] + d[2] * i;

		Point2f tempd = (point[1] - point[0]) / 8;
		for (int j = 0; j <= 8; j++) {
			corners.push_back(point[0] + tempd * j);
		}
	}

	return corners;
}

vector<Block> ImageProcessor::findBlocks(vector<Point2f> corners) {
	vector<Block> blocks;

	int count = 0;
	vector<Point2f> block;
	for (int i = 0; i < 8; i++) {
		int index = 9 * i;
		for (int j = 0; j < 8; j++) {
			block.push_back(corners[index + j]);
			block.push_back(corners[index + j + 1]);
			block.push_back(corners[index + j + 9]);
			block.push_back(corners[index + j + 10]);

			blocks.push_back(Block(count, block));
			block.clear();
			count++;
		}
	}
	return blocks;
}

vector<Block> ImageProcessor::findChessboardBlocks(String title) {
	Mat input_gray_image = imread(title, IMREAD_GRAYSCALE);

	Mat result_otsu_image;
	threshold(input_gray_image, result_otsu_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
	
	Mat biggestBlob = findBiggestBlob(result_otsu_image);

	Mat contours;
	Canny(biggestBlob, contours, 125, 350);

	// 선 감지 위한 허프 변환
	vector<cv::Vec2f> lines;
	HoughLines(contours, lines, 1, PI / 180, 180);  // 투표(vote) 최대 개수

	vector<Point2f> intersections = findIntersection(lines, input_gray_image.cols, input_gray_image.rows);
	vector<Point2f> edges = findEdge(intersections);

	for (int i = 0; i < 4; i++) circle(input_gray_image, Point(edges[i]), 5, Scalar(255), 1, 8, 0);

	vector<Point2f> corners = calculateCorners(edges);
	for (vector<Point2f>::iterator i = corners.begin(); i != corners.end(); i++) circle(input_gray_image, Point((*i)), 5, Scalar(255), 1, 8, 0);

	vector<Block> blocks = findBlocks(corners);
	return blocks;
}

vector<Block> ImageProcessor::findColorObject(vector<Block> blocks, String title, int color) {
	Mat image = imread(title);
	imshow("원본이미지", image);

	Mat threshold_image;
	Mat point_image;

	if (color == 0) {
		threshold(image, threshold_image, 240, 255, THRESH_BINARY);
		threshold(image, point_image, 240, 255, THRESH_BINARY);
	}
	else {
		threshold(image, threshold_image, 20, 255, THRESH_BINARY_INV);
		threshold(image, point_image, 20, 255, THRESH_BINARY_INV);
	}
	imshow("threshold 이미지", threshold_image);

	vector<Block> objects;

	int index = 0;
	for (vector<Block>::iterator iter = blocks.begin(); iter != blocks.end(); iter++) {
		float x_h, x_l, y_h, y_l;
		Block block = *iter;
		vector<Point2f> points = block.getPoints();
		x_l = points.at(0).x < points.at(2).x ? points.at(0).x : points.at(2).x;
		x_h = points.at(1).x < points.at(3).x ? points.at(1).x : points.at(3).x;
		y_l = points.at(0).y < points.at(1).y ? points.at(0).y : points.at(1).y;
		y_h = points.at(2).y < points.at(3).y ? points.at(2).y : points.at(3).y;

		int count = 0;
		for (int x = x_l; x < x_h; x++) {
			for (int y = y_l; y < y_h; y++) {
				int color = threshold_image.at<Vec3b>(y, x)[0];
				if (color != 0) {
					count++;
					circle(point_image, Point(x, y), 5, Scalar(255), 1, 8, 0);
				}
			}
		}
		printf("%d, %d\n", index++, count);

		if (count > 80) {
			(*iter).setIsInObject(true);
			objects.push_back(*iter);
		}
	}
	waitKey(0);

	return objects;
}

vector<Block> ImageProcessor::findChessObject(vector<Block> blocks, String title) {
	vector<Block> w_objects = findColorObject(blocks, title, WHITE);
	vector<Block> b_objects = findColorObject(blocks, title, BLACK);

	vector<Block> objects = w_objects;
	objects.insert(objects.end(), b_objects.begin(), b_objects.end());
	Mat image = imread(title);

	for (vector<Block>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
		for (int i = 0; i < 4; i++) circle(image, Point((*iter).getPoints()[i]), 5, Scalar(255), 1, 8, 0);
	}

	cv::namedWindow("Detected edge point");
	cv::imshow("Detected edge point", image);

	waitKey(0);
	return objects;
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