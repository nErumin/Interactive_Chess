#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/opencv.hpp>
#include <thread>
#include <iostream>

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
	void startVideo(int devicdId);
	void captureImage();
	~Video() = default;
};

#endif // OBSERVER_H