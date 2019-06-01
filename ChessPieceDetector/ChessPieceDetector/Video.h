#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>
#include "Observable.h"

#include "ImageProcessor.h"

#include <TransmissionService.h>

#define ENTER_KEY 0xd
#define ESC_KEY 0x1b

using namespace cv;
using namespace std;

#define EXIT -1
#define CAPTURE 0
#define TEST 1

class Video : public Observable<String>
{
public:
	Video();
	void takeVideo(int devicdId);
	void takeVideo(int devicdId, Network::TransmissionService& service);
	void captureImage();
	void showImage(String image_name);

	~Video() = default;
private:
	Mat frame;
	VideoCapture cap;
	bool end_video;
};

#endif // OBSERVER_Hd