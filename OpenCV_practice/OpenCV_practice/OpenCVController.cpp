#include "OpenCVController.h"

OpenCVController::OpenCVController() {
	video.registerObserver(this);
}

void OpenCVController::notify(String&& img_name) {
	cout << "hi" << endl;
	thread t;
	t = thread(&OpenCVController::startImageProcessor, this);
	t.detach();
}

void OpenCVController::startVideo() {
	//video.showImage("test.jpg");
	video.takeVideo(0);
}

void OpenCVController::startImageProcessor() {
	cout << "h2" << endl;
	processor.detectAndDrawChessboardCorners("wood_chess_board.jpg");
}