#include "OpenCVController.h"

OpenCVController::OpenCVController() {
	video.registerObserver(this);
}

void OpenCVController::notify(String&& img_name) {
	cout << "notify!!" << endl;

	String filePath = DEFAULT_PATH + img_name;
	Mat image = imread(filePath);

	// Check for failure
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
	}

	String windowName = "Image"; //Name of the window

	namedWindow(windowName); // Create a window

	imshow(windowName, image); // Show our image inside the created window.

	waitKey(0); // Wait for any keystroke in the window

	destroyWindow(windowName); //destroy the created window
	// the camera will be deinitialized automatically in VideoCapture destructor
}

void OpenCVController::startVideo() {
	//video.showImage("test.jpg");
	video.takeVideo(0);
}

void OpenCVController::startImageProcessor() {
	processor.detectAndDrawChessboardCorners("wood_chess_board.jpg");
}