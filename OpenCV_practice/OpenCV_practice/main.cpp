#include "OpenCVController.h"
#include <chrono>

#include <Server.h>
#include <SocketConnection.h>
#include <Address.h>

int main() {
	
	ImageProcessor ip;
	String img_name = "test.jpg";
	chrono::system_clock::time_point start = chrono::system_clock::now();

	if (ip.isFirst()) ip.initialize(img_name);
	else ip.recognizeMovement(img_name);

	chrono::duration<double> sec = chrono::system_clock::now() - start;
	start = chrono::system_clock::now();
	cout << "1. initial - corner detection and find object" << endl << "elapsed time : " << sec.count() << endl << endl;
	
	ip.recognizeMovement(img_name);
	sec = chrono::system_clock::now() - start;
	cout << "2. just find object" << endl << "elapsed time : " << sec.count() << endl;

	/*
	Network::Server server{ Network::Address("", 8000) };

	auto connection = server.waitClient();

	OpenCVController controller(connection);
	controller.startVideo(1);
	*/
	return 0;
}
