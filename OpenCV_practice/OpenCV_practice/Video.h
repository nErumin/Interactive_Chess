#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>

#include "DefaultPath.h"

#define ENTER_KEY 0xd
#define ESC_KEY 0x1b

using namespace cv;
using namespace std;

class Video
{
private:
	Mat frame;
	VideoCapture cap;
	bool end_video;

public:
	Video();
	void takeVideo(int devicdId);
	void captureImage();
	void showImage(String image_name);
	~Video() = default;
};

#endif // OBSERVER_H