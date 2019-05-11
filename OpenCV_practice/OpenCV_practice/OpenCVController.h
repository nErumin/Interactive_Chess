#ifndef OPENCVCONTROLLER_H
#define OPENCVCONTROLLER_H

#include "ImageProcessor.h"
#include "Video.h"
#include "Observer.h"
#include "Vector2.h"
#include <thread>

class OpenCVController : public Observer<String>,
						 public Observer<Vector2>
{
public:
	OpenCVController();
	void startVideo(int id);
	void startImageProcessor(String img_name);
	void notify(String&& img_name) override;
	void notify(Vector2&& location) override;

	~OpenCVController() = default;
private:
	ImageProcessor processor;
	Video video;
};


#endif