#include "Video.h"
#include <thread>
#include <chrono>

Video::Video()
	:end_video { false }
{
}

void Video::takeVideo(int deviceId) {

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
		rectangle(frame, Point(160, 30), Point(580, 460), Scalar(255, 0, 0) /*blue*/, 10, 8, 0);
		// check if we succeeded
		if (frame.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
		}
		// show live and wait for a key with timeout long enough to show images
		imshow("Live", frame);

		//todo server receive 
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

void Video::takeVideo(int deviceId, Network::TransmissionService& service) {

	int apiId = cv::CAP_ANY;      // 0 = autodetect default API
	cap.open(deviceId + apiId);	  // open selected camera using selected API
	
	// check if we succeeded
	if (!cap.isOpened()) {
		cerr << "ERROR! Unable to open camera\n";
	}

	cout << "Net version video" << endl;

	//--- GRAB AND WRITE LOOP
	cout << "Start grabbing" << endl
		<< "Press enter key to take a capture" << endl
		<< "Press esc key to terminate" << endl;


	int inkey = 1;
	auto receiveThread = std::thread([this, &inkey, &service]
	{
		cout << "Wait for you!" << endl;

		while (!end_video)
		{
			inkey = std::stoi(service.receive(128));
			cout << "WOW! I GOT " << inkey << endl;
		}
	});

	receiveThread.detach();

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
		
		//todo server receive
		waitKey(25);

		if (inkey == EXIT) {
			break;
		}
		else if (inkey == CAPTURE) {
			captureImage();
			inkey = 1;
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
	
	// 관심영역 설정 (set ROI (X, Y, W, H)).
	Rect rect(160, 30, 420, 430);

	// 관심영역 자르기 (Crop ROI).
	Mat subImage = save_img(rect);

	// Save the frame into a file
	imwrite("origin.jpg", save_img); // A JPG FILE IS BEING SAVED
	imwrite("test.jpg", subImage); // A JPG FILE IS BEING SAVED
	showImage("test.jpg");
	
	notifyToObservers("test.jpg");
}

void Video::showImage(String image_name) {
	
	String filePath = image_name;
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
