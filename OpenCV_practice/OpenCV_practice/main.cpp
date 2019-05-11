#include "OpenCVController.h"
#include <chrono>

int main() {
	//OpenCVController controller;
	//controller.startVideo(1);
	
	
	ImageProcessor ip;
	String img_name = "test.jpg";
	
	chrono::system_clock::time_point start = chrono::system_clock::now();
	
	if (ip.isFirst()) ip.initialize(img_name);
	else ip.recognizeMovement(img_name);
	
	chrono::duration<double> sec = chrono::system_clock::now() - start;
	cout << "elapsed time : " << sec.count() << endl;
	
	return 0;
}
