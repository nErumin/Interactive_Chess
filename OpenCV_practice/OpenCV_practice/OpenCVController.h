#ifndef OPENCVCONTROLLER_H
#define OPENCVCONTROLLER_H

#include "ImageProcessor.h"
#include "Video.h"
#include "Observer.h"
#include "Vector2.h"
#include <thread>
#include <TransmissionService.h>
#include <SocketConnection.h>
#include <memory>

class OpenCVController : public Observer<String>,
						 public Observer<String, int>
{
public:
	OpenCVController();
	OpenCVController(Network::SocketConnection& connection);
	void startVideo(int id);
	void startImageProcessor(String img_name);
	void notify(String&& img_name) override;
	void notify(String&& msg, int&& temp) override;

	~OpenCVController() = default;

private:
	ImageProcessor processor;
	Video video;
	std::unique_ptr<Network::TransmissionService> service;
};


#endif