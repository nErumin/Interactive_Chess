#include "Video.h"
#include "ImageProcessor.h"
#include <thread>

void startVideo();
void startImageProcessor();

int main(int, char**)
{
	thread t1, t2;
	t1 = thread(startVideo);
	t2 = thread(startImageProcessor);
	t1.join();
	t2.join();
	return 0;
}

void startVideo() {
	Video video;
	video.showImage("test.jpg");
	video.takeVideo(0);
}

void startImageProcessor() {
	ImageProcessor processor;
	processor.detectAndDrawChessboardCorners("wood_chess_board.jpg");
}