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
	cout << "initial - corner detection and find object" << endl << "elapsed time : " << sec.count() << endl << endl;
	
	/*
	Network::Server server{ Network::Address("", 8000) };

	auto connection = server.waitClient();
	
	OpenCVController controller(connection);
	*/
	//OpenCVController controller = OpenCVController();
	//controller.startVideo(1);
	
	return 0;
}
