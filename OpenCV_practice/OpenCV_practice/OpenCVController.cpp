#include "OpenCVController.h"

OpenCVController::OpenCVController(Network::SocketConnection& connection) : service(connection) {
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

void OpenCVController::startVideo(int id) {
	video.takeVideo(id);
}

void OpenCVController::startImageProcessor(String img_name) {
	cout << img_name << endl;
	if (processor.isFirst()) processor.initialize(img_name);
	else processor.recognizeMovement(img_name);
}