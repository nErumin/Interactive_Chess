#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/calib3d.hpp"

using namespace cv;
using namespace std;

int takeVideo();
int captureImage(VideoCapture cap);
int displayImage(String imageName);

bool detectAndDrawChessboardCorners();

int main(int, char**)
{

	//takeVideo();

	//displayImage("wood_chess_board.jpg");

	detectAndDrawChessboardCorners();
	return 0;
}

int takeVideo() {
	Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;
	// open the default camera using default API
	cap.open(0);
	// OR advance usage: select any API backend
	int deviceID = 0;             // 0 = open default camera
	int apiID = cv::CAP_ANY;      // 0 = autodetect default API
								  // open selected camera using selected API
	cap.open(deviceID + apiID);
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl
		<< "Press any key to terminate" << endl;
	for (;;)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			return -2;
		}
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", frame);
		if (waitKey(5) >= 0) {
			captureImage(cap);
			break;
		}
	}

	return 0;
}

int captureImage(VideoCapture cap) {
	Mat save_img;

	cap >> save_img;

	String title;

	cin >> title;
	cout << "tlte : " << title << endl;

	if (save_img.empty())
	{
		std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
		cin.get(); //wait for any key press
		return -1;
	}
	// Save the frame into a file
	imwrite(title, save_img); // A JPG FILE IS BEING SAVED

	return 0;
}

int displayImage(String imageName) {
	String defaultPath = "D:/Documents/2019_1_cau/Capstone2019/Interactive_Chess/OpenCV_practice/OpenCV_practice/";

	String filePath = defaultPath + imageName;
	Mat image = imread(filePath);

	// Check for failure
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		cin.get(); //wait for any key press
		return -1;
	}

	String windowName = "Test"; //Name of the window

	namedWindow(windowName); // Create a window

	imshow(windowName, image); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window
	// the camera will be deinitialized automatically in VideoCapture destructor

	return 0;
}

bool detectAndDrawChessboardCorners()
{
	Mat img = imread("D:/Documents/2019_1_cau/Capstone2019/Interactive_Chess/OpenCV_practice/OpenCV_practice/wood_chess_board.jpg");
	imshow("image", img);
	//moveWindow("image", 40, 40);

	Size patternsize(7, 7); //interior number of corners
	Mat gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);//source image
	vector<Point2f> corners; //this will be filled by the detected corners

	//CALIB_CB_FAST_CHECK saves a lot of time on images
	//that do not contain any chessboard corners
	bool patternfound = findChessboardCorners(gray, patternsize, corners,
		CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
		+ CALIB_CB_FAST_CHECK);

	float distances[6][6][2] = { 0, };
	for (int i = 0, k = 0; i < 6; i++, k += 1) {
		for (int j = 0; j < 6; j++) {
			distances[i][j][0] = corners[(6 * i) + j + k].x - corners[(6 * i) + j + k + 1].x;
			distances[i][j][1] = corners[(6 * i) + j].y - corners[(6 * i) + j + 7].y;
		}
	}

	float mean_distance_x = 0;
	float mean_distance_y = 0;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			mean_distance_x += distances[i][j][0];
			mean_distance_y += distances[i][j][1];
		}
	}
	mean_distance_x /= 36;
	mean_distance_y /= 36;

	drawChessboardCorners(img, patternsize, Mat(corners), patternfound);

	imshow("result", img);
	moveWindow("result", img.cols / 2, 100);
	waitKey(0);
	return true;
}