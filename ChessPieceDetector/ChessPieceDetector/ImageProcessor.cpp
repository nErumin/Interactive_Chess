#include "ImageProcessor.h"
#include <chrono>
#include <thread>
#include <algorithm>

ImageProcessor::ImageProcessor() {
	
}

Mat ImageProcessor::thresholdImage(Mat image) {
	Mat threshold_image;

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
	drawContours(contoursImage, contours, largest_contour_index, Scalar(255), FILLED, 8, hierarchy); // Draw the largest contour using previously stoGREEN index.
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
	const float scaler = 0.0215;
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
	for (vector<Point2f>::iterator i = points.begin(); i != points.end(); i++) circle(image, Point((*i)), 5, Scalar(0), FILLED, 8, 0);
	imshow(title, image);
}

vector<Block> ImageProcessor::findChessboardBlocks(String title) {
	//read image with gray scale
	Mat input_gray_image = imread(title, IMREAD_GRAYSCALE);
	
#if TEST == 1
	imshow("gray", input_gray_image);
#endif
	
	//threshold colors
	Mat threshold_image = thresholdImage(input_gray_image);
#if TEST == 1
	imshow("threshold", threshold_image);
#endif

	//find biggest area in image
	Mat biggestBlob = findBiggestBlob(threshold_image);
#if TEST == 1
	imshow("biggest Blob", biggestBlob);
#endif

	//Canny algo for finding contours;
	Mat contours;
	Canny(biggestBlob, contours, 125, 350);
#if TEST == 1
	imshow("canny", contours);
#endif

	//Hough transformation for finding lines
	vector<Vec2f> lines;
	HoughLines(contours, lines, 1, PI / 180, 100);  // 투표(vote) 최대 개수
#if TEST == 1
	Mat lines_image;
	input_gray_image.copyTo(lines_image);
	drawLinesInImage(lines_image, contours, lines);
#endif

	//find intersections
	vector<Point2f> intersections = findIntersection(lines, input_gray_image.cols, input_gray_image.rows);
#if TEST == 1
	Mat intersections_image;
	lines_image.copyTo(intersections_image);
	drawPointsInImage(intersections_image, intersections, "intersections");
#endif	

	//find edge points
	vector<Point2f> edges = findEdge(intersections);
#if TEST == 1
	Mat edges_image;
	input_gray_image.copyTo(edges_image);
	drawPointsInImage(edges_image, edges, "edges");
#endif

	//adjust edge points
	vector<Point2f> adjust_edges = adjustEdgePosition(edges);
#if TEST == 1
	Mat adjust_image;
	edges_image.copyTo(adjust_image);
	drawPointsInImage(adjust_image, adjust_edges, "adjust_edges");
#endif

	//find all corners in chessboard
	vector<Point2f> corners = calculateCorners(adjust_edges);
#if TEST == 1
	Mat corners_image;
	input_gray_image.copyTo(corners_image);
	drawPointsInImage(corners_image, corners, "corners");

	waitKey(0);
	destroyAllWindows();
#endif

	//find all blocks in chessboard
	vector<Block> blocks = findBlocks(corners);
	return blocks;
}

void detectHSColor(const Mat& image, double minHue, double maxHue, double minSat, double maxSat, double minValue, double maxValue, Mat& mask) {
	Mat hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);

	vector<Mat> channels;
	split(hsv, channels);

#if TEST == 1
	imshow("hue", channels[0]);
	imshow("sat", channels[1]);
	imshow("value", channels[2]);
#endif

	Mat threshold;
	inRange(hsv, Scalar(minHue, minSat, minValue), Scalar(maxHue, maxSat, maxValue), threshold);

	mask = threshold;
}

vector<Block> ImageProcessor::findColorObject(String title, int COLOR) {
	Mat image = imread(title);

#if TEST == 1
	imshow("origin", image);
#endif

	Mat threshold_image;
	int sub = -10, add = 15;

	if (COLOR == GREEN) {
		detectHSColor(image, 50, 100, 50, 255, 0, 120, threshold_image);
	}
	else {
		detectHSColor(image, 100, 150, 100, 255, 110, 255, threshold_image);
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
		for (int x = x_l + 2; x < x_h - 2; x++) {
			for (int y = y_l + 2; y < y_h - 2; y++) {
				int color = threshold_image.at<uchar>(y, x);
				if (color != 0) {
					count++;
				}
			}
		}
#if TEST == 1
		printf("%d, %d\n", index++, count);
#endif

		if (count > 1000) {
			(*iter).setIsInObject(true);
			objects.push_back(*iter);
		}
	}

#if TEST == 1
	imshow("threshold", threshold_image);
	waitKey(0);
	destroyAllWindows();
#endif

	return objects;
}

void ImageProcessor::showChessObject(String title) {
	Mat image = imread(title);

	for (vector<Block>::iterator iter = this->black_pieces.begin(); iter != this->black_pieces.end(); iter++) {
		circle(image, Point((*iter).getCenterPoint()), 5, Scalar(0, 255, 0), FILLED, 8, 0);
	}
	for (vector<Block>::iterator iter = this->white_pieces.begin(); iter != this->white_pieces.end(); iter++) {
		circle(image, Point((*iter).getCenterPoint()), 5, Scalar(255, 0, 0), FILLED, 8, 0);
	}

	namedWindow("Detected Object");
	imshow("Detected Object", image);

	waitKey(0);
	destroyAllWindows();
}

bool ImageProcessor::isFirst() {
	if (this->first) {
		if (this->black_pieces.size() == 16 && this->white_pieces.size() == 16) {
			this->first = false;
			return false;
		}
		return true;
	}
	else return false;
}

void ImageProcessor::initialize(String title) {
	this->blocks = findChessboardBlocks(title);
	
	this->white_pieces = findColorObject(title, BLUE);
	this->black_pieces = findColorObject(title, GREEN);
	
	String msg;
	if (this->white_pieces.size() != 16) {
		msg = "White pieces are not initial setting";
		showChessObject(title);
	}
	else if (this->black_pieces.size() != 16) {
		msg = "Black pieces are not initial setting";
		showChessObject(title);
	}
	else {
		if (this->black_pieces.at(0).getIndex() == 0) msg = "BLACK";
		else msg = "WHITE";

#if TEST == 1
		showChessObject(title);
#endif
	}
	notifyToObservers(String(msg), 0);
}

vector<int> duplication(vector<int>& a, vector <int>& b) {
	vector<int>::iterator iter;
	vector<int>::iterator iter_b;
	vector<int> c = a; //a의 값 복사

	for (iter_b = b.begin(); iter_b != b.end(); iter_b++) {
		for (iter = c.begin(); iter != c.end();) {
			if (*iter == *iter_b)
				iter = c.erase(iter); //중복 제거
			else
				iter++;
		}
	}
	return c; //결과 반환
}

vector<int> ImageProcessor::comparePieces(vector<Block> previous_pieces, vector<Block> new_pieces) {
	vector<int> previous_index;
	vector<int> new_index;

	for (vector<Block>::iterator i = previous_pieces.begin(); i != previous_pieces.end(); i++) previous_index.push_back((*i).getIndex());
	for (vector<Block>::iterator i = new_pieces.begin(); i != new_pieces.end(); i++) new_index.push_back((*i).getIndex());


	sort(previous_index.begin(), previous_index.end());
	sort(new_index.begin(), new_index.end());

	vector<int> pdif_index, ndif_index;
	pdif_index = duplication(previous_index, new_index);
	ndif_index = duplication(new_index, previous_index);
	pdif_index.insert(pdif_index.end(), ndif_index.begin(), ndif_index.end());

	return pdif_index;
}

String indexToPoint(int index) {
	String point;
	
	point += to_string(index % 8);
	point += ',';
	point += to_string(index / 8);

	return point;
}

String makeProtocolString(vector<int> black_index, vector<int> white_index) {
	int count = 0;
	count += black_index.size()/2;
	count += white_index.size()/2;

	String msg = "";
	msg += to_string(count);
	msg += ":";
	if (black_index.size() == 2) {
		for (vector<int>::iterator i = black_index.begin(); i != black_index.end(); i++) {
			msg += (indexToPoint(*i) + "$");
		}
		msg.pop_back();
		msg += '|';
	}
	if (white_index.size() == 2) {
		for (vector<int>::iterator i = white_index.begin(); i != white_index.end(); i++) {
			msg += (indexToPoint(*i) + "$");
		}
	}
	msg.pop_back();

	cout << msg << endl;
	return msg;
}

void ImageProcessor::recognizeMovement(String title) {
	vector<Block> new_white_pieces = findColorObject(title, BLUE);
	vector<Block> new_black_pieces = findColorObject(title, GREEN);

	vector<int> white_dif_indexs = comparePieces(this->white_pieces, new_white_pieces);
	vector<int> black_dif_indexs = comparePieces(this->black_pieces, new_black_pieces);

	String msg = makeProtocolString(black_dif_indexs, white_dif_indexs);

	this->black_pieces = new_black_pieces;
	this->white_pieces = new_white_pieces;

#if TEST == 1
	showChessObject(title);
#endif

	notifyToObservers(String(msg), 0);
}
