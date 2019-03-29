#ifndef VIDEO_H
#define VIDEO_H

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class Video
{
private:
	Mat frame;
	//--- INITIALIZE VIDEOCAPTURE
	VideoCapture cap;

public:
	virtual void startVideo(int devicdID) = 0;
	virtual void captureImage() = 0;
	virtual ~Video() = default;
};

#endif // OBSERVER_H