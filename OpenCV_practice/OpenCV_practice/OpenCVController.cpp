#include "OpenCVController.h"

OpenCVController::OpenCVController() {
	video.registerObserver(this);
	processor.registerObserver(this);
}

void OpenCVController::notify(String&& img_name) {
	thread t;
	t = thread(&OpenCVController::startImageProcessor, this, img_name);
	t.detach();
}

void OpenCVController::notify(Vector2&& location) {
	cout << location.x() << ", " << location.y() << endl;
	// to do
	// response to chess system
}

void OpenCVController::startVideo() {
	video.takeVideo(0);
}

void OpenCVController::startImageProcessor(String img_name) {
	cout << img_name << endl;
	processor.detectAndDrawChessboardCorners(img_name);
}