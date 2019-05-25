#include "OpenCVController.h"

OpenCVController::OpenCVController(){ 
	video.registerObserver(this);
	processor.registerObserver(this);
}


OpenCVController::OpenCVController(Network::SocketConnection& connection) 
	: service(std::make_unique<Network::TransmissionService>(connection)) {
	video.registerObserver(this);
	processor.registerObserver(this);
}

void OpenCVController::notify(String&& img_name) {
#ifdef NETWORK
	service->send("OK");
#endif
	thread t;
	t = thread(&OpenCVController::startImageProcessor, this, img_name);
	t.detach();
}

void OpenCVController::notify(Vector2&& location) {
	cout << location.x() << ", " << location.y() << endl;
	
	// to do
	// response to chess system
#ifdef NETWORK
	service->send("1:0,1$0,3");
#endif
}

void OpenCVController::startVideo(int id) {
#ifdef NETWORK
	video.takeVideo(id, *service);
#else
	video.takeVideo(id);
#endif
}

void OpenCVController::startImageProcessor(String img_name) {
	cout << img_name << endl;
	if (processor.isFirst()) processor.initialize(img_name);
	else processor.recognizeMovement(img_name);
}