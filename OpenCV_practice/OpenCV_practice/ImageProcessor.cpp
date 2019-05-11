#include "ImageProcessor.h"
#include <chrono>
#include <thread>

ImageProcessor::ImageProcessor() {
	
}

Mat ImageProcessor::thresholdImage(Mat image) {
	Mat threshold_image;
	//이진화를 한다.
	//threshold(image, threshold_image, 0, 255, THRESH_BINARY | THRESH_OTSU);
	adaptiveThreshold(image, threshold_image, 101, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 101, 20);

	return threshold_image;
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
			//cout << "point: (" << point.x << "," << point.y << ")\n";
		}
	}
	return points;
}

vector<Point2f> ImageProcessor::findEdge(vector<Point2f> points) {
	vector<Point2f> edges;
	Point point[4];
	float scaler = 0.9;
	if (points.size() != 0) {
		for (int i = 0; i < 4; i++) point[i] = points[0];
		for (vector<Point2f>::iterator i = points.begin() + 1; i != points.end(); i++) {
			float x = (*i).x, y = (*i).y;
			float check = x + y;
			if (point[3].x * (-1) + point[3].y * (-1) > x * (-1) + y * (-1)) {
				point[3] = *i;
			}
			if (point[1].x * (-1) + point[1].y > x * (-1) + y) {
				point[1] = *i;
			}
			if (point[2].x + point[2].y * (-1) > x + y * (-1)) {
				point[2] = *i;
			}
			if (point[0].x + point[0].y > x + y) {
				point[0] = *i;
			}
		}
		for (int i = 0; i < 4; i++) edges.push_back(point[i]);
	}
	return edges;
}

vector<Point2f> adjustEdgePosition(vector<Point2f> edges) {
	const float scaler = 0.07;
	float dx[2] = { (edges[1].x - edges[0].x) * scaler, (edges[3].x - edges[2].x) * scaler };
	float dy[2] = { (edges[2].y - edges[0].y) * scaler, (edges[3].y - edges[1].y) * scaler };

	const int order[2][4] = { {0, 0, 1, 1}, {0, 1, 0, 1} };
	const bool sub[2][4] = { {false, true, false, true}, {false, false, true, true} };
	
	vector<Point2f> adjust_edges;
	for (int i = 0; i < 4; i++) {
		float tdx = dx[order[0][i]], tdy = dy[order[1][i]];
		if (sub[0][i]) tdx *= -1;
		if (sub[1][i]) tdy *= -1;
		adjust_edges.push_back(Point2f(edges[i].x + tdx, edges[i].y + tdy));
	}
	return adjust_edges;
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

			Block temp = Block(count, block);
			temp.setCenterPoint();
			blocks.push_back(temp);
			block.clear();
			count++;
		}
	}
	return blocks;
}

void drawLinesInImage(Mat image, Mat contours, vector<Vec2f> lines) {
	Mat result(contours.rows, contours.cols, CV_8U, Scalar(255));

	// 선 벡터를 반복해 선 그리기
	vector<Vec2f>::const_iterator it = lines.begin();
	while (it != lines.end()) {
		float rho = (*it)[0];   // 첫 번째 요소는 rho 거리
		float theta = (*it)[1]; // 두 번째 요소는 델타 각도
		if (theta < PI / 4. || theta > 3.*PI / 4.) { // 수직 행
			Point pt1(rho / cos(theta), 0); // 첫 행에서 해당 선의 교차점   
			Point pt2((rho - result.rows*sin(theta)) / cos(theta), result.rows);
			// 마지막 행에서 해당 선의 교차점
			line(image, pt1, pt2, Scalar(255), 1); // 하얀 선으로 그리기
		}
		else { // 수평 행
			Point pt1(0, rho / sin(theta)); // 첫 번째 열에서 해당 선의 교차점  
			Point pt2(result.cols, (rho - result.cols*cos(theta)) / sin(theta));
			// 마지막 열에서 해당 선의 교차점
			line(image, pt1, pt2, Scalar(255), 1); // 하얀 선으로 그리기
		}
		++it;
	}
	imshow("lines", image);
}

void drawPointsInImage(Mat image, vector<Point2f> points, String title) {
	for (vector<Point2f>::iterator i = points.begin(); i != points.end(); i++) circle(image, Point((*i)), 5, Scalar(255), 1, 8, 0);
	imshow(title, image);
}

vector<Block> ImageProcessor::findChessboardBlocks(String title) {
	//read image with gray scale
	Mat input_gray_image = imread(title, IMREAD_GRAYSCALE);
	//imshow("gray", input_gray_image);

	//threshold colors
	Mat threshold_image = thresholdImage(input_gray_image);
	//imshow("threshold", threshold_image);

	//find biggest area in image
	Mat biggestBlob = findBiggestBlob(threshold_image);
	//imshow("biggest Blob", biggestBlob);

	//Canny algo for finding contours;
	Mat contours;
	Canny(biggestBlob, contours, 125, 350);
	//imshow("canny", contours);

	//Hough transformation for finding lines
	vector<Vec2f> lines;
	HoughLines(contours, lines, 1, PI / 180, 120);  // 투표(vote) 최대 개수
	//drawLinesInImage(input_gray_image, contours, lines);

	//find intersections
	vector<Point2f> intersections = findIntersection(lines, input_gray_image.cols, input_gray_image.rows);
	
	//find edge points
	vector<Point2f> edges = findEdge(intersections);
	//drawPointsInImage(input_gray_image, edges, "edges");

	//adjust edge points
	vector<Point2f> adjust_edges = adjustEdgePosition(edges);
	//drawPointsInImage(input_gray_image, adjust_edges, "adjust_edges");

	//find all corners in chessboard
	vector<Point2f> corners = calculateCorners(adjust_edges);
	//drawPointsInImage(input_gray_image, corners, "corners");

	//waitKey(0);
	//destroyAllWindows();

	//find all blocks in chessboard
	vector<Block> blocks = findBlocks(corners);
	return blocks;
}

vector<Block> ImageProcessor::findColorObject(String title, int color) {
	Mat image = imread(title);
	//imshow("origin", image);

	Mat threshold_image;
	//Mat point_image;
	int sub = -10, add = 15;
	if (color == WHITE) {
		inRange(image, Scalar(average_black[0] + sub, average_black[1] + sub, average_black[2] + sub), Scalar(average_black[0] + add, average_black[1] + add, average_black[2] + add), threshold_image);
		//inRange(image, Scalar(average_black[0] + sub, average_black[1] + sub, average_black[2] + sub), Scalar(average_black[0] + add, average_black[1] + add, average_black[2] + add), point_image);
		//threshold(image, threshold_image, 240, 255, THRESH_BINARY);
		//threshold(image, point_image, 240, 255, THRESH_BINARY);
	}
	else {
		inRange(image, Scalar(average_white[0] + sub, average_white[1] + sub, average_white[2] + sub), Scalar(average_white[0] + add, average_white[1] + add, average_white[2] + add), threshold_image);
		//inRange(image, Scalar(average_white[0] + sub, average_white[1] + sub, average_white[2] + sub), Scalar(average_white[0] + add, average_white[1] + add, average_white[2] + add), point_image);
		//threshold(image, threshold_image, 20, 255, THRESH_BINARY_INV);
		//threshold(image, point_image, 20, 255, THRESH_BINARY_INV);
	}

	vector<Block> objects;

	int index = 0;
	for (vector<Block>::iterator iter = this->blocks.begin(); iter != this->blocks.end(); iter++) {
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
				int color = threshold_image.at<uchar>(y, x);
				if (color != 0) {
					count++;
					//circle(point_image, Point(x, y), 5, Scalar(255), 1, 8, 0);
				}
			}
		}
		//printf("%d, %d\n", index++, count);

		if (count > 50) {
			(*iter).setIsInObject(true);
			objects.push_back(*iter);
		}
	}

	//imshow("threshold", threshold_image);
	//imshow("finding point", point_image);
	//waitKey(0);

	return objects;
}

vector<Block> ImageProcessor::findChessObject(String title) {
	vector<Block> w_objects = findColorObject(title, WHITE);
	vector<Block> b_objects = findColorObject(title, BLACK);

	vector<Block> objects = w_objects;
	objects.insert(objects.end(), b_objects.begin(), b_objects.end());
	Mat image = imread(title);

	for (vector<Block>::iterator iter = objects.begin(); iter != objects.end(); iter++) {
		circle(image, Point((*iter).getCenterPoint()), 5, Scalar(255), 1, 8, 0);
	}

	//namedWindow("Detected edge point");
	//imshow("Detected edge point", image);

	//waitKey(0);
	//destroyAllWindows();
	return objects;
}

bool ImageProcessor::isFirst() {
	if (blocks.size() == 0) return true;
	else return false;
}

void ImageProcessor::setAverageColor(String title) {
	Mat image = imread(title);
	
	int black[16][3], white[16][3];
	//int color = image.at<Vec3b>(y, x)[0];
	
	//inRange(image, average_black, average_white, image);
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			Point2f wblock_cp = this->blocks.at(i).getCenterPoint();
			Point2f bblock_cp = this->blocks.at(i + 48).getCenterPoint();
			white[i][j] = image.at<Vec3b>(wblock_cp.y, wblock_cp.x)[j];
			black[i][j] = image.at<Vec3b>(bblock_cp.y, bblock_cp.x)[j];
		}
	}
	int average_b[3] = { 0, }, average_w[3] = { 0, };
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 3; j++) {
			average_b[j] += black[i][j];
			average_w[j] += white[i][j];
		}
	}
	for (int i = 0; i < 3; i++) {
		average_b[i] /= 16;
		this->average_black[i] = average_b[i];
		average_w[i] /= 16;
		this->average_white[i] = average_w[i];
	}
}

void ImageProcessor::initialize(String title) {
	this->blocks = findChessboardBlocks(title);
	setAverageColor(title);
	
	this->pieces = findChessObject(title);
}

void ImageProcessor::recognizeMovement(String title) {
	vector<Block> newPieces = findChessObject(title);

	notifyToObservers(Vector2(1.0, 2.0));
}
