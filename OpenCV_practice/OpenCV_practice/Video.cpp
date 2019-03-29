#include "Video.h"
Video::Video()
	:end_video { false }{ }

void Video::startVideo(int deviceId) {

	int apiId = cv::CAP_ANY;      // 0 = autodetect default API
	cap.open(deviceId + apiId);	  // open selected camera using selected API
	
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
	}

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl
		<< "Press enter key to take a capture" << endl
		<< "Press esc key to terminate" << endl;

	while (!end_video)
	{
		// wait for a new frame from camera and store it into 'frame'
		cap.read(frame);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
		}
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", frame);

		int inkey = waitKey(25);
		if (inkey == ESC_KEY) {
			break;
		}
		else if (inkey == ENTER_KEY) {
			captureImage();
			cout << "take a capture" << endl;
		}
	}
}

void Video::captureImage() {
	Mat save_img;

	Video::cap >> save_img;


	if (save_img.empty())
	{
		std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
	}
	// Save the frame into a file
	imwrite("test.jpg", save_img); // A JPG FILE IS BEING SAVED
}
