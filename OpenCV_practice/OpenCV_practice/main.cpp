#include "OpenCVController.h"

int main() {
	/*
	OpenCVController controller;
	controller.startVideo(1);
	*/
	
	ImageProcessor ip;
	String img_name = "test.jpg";
	if (ip.isFirst()) ip.initialize(img_name);
	else ip.recognizeMovement(img_name);
	
	return 0;
}
