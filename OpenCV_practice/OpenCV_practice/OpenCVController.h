#ifndef OPENCVCONTROLLER_H
#define OPENCVCONTROLLER_H

#include "ImageProcessor.h"
#include "Video.h"
#include "Observer.h"
#include <thread>

class OpenCVController : public Observer<String>
{
public:
	OpenCVController();
	void startVideo();
	void startImageProcessor();
	void notify(String&& img_name) override;
	~OpenCVController() = default;

private:
	ImageProcessor processor;
	Video video;
};


#endif