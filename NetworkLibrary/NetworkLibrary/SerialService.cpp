#include "SerialService.h"
#include "TransmissionInternals.h"
#include <boost/asio/serial_port.hpp>
#include <boost/asio/io_service.hpp>

using namespace Network;

class SerialService::SerialServiceImpl
{
public:
    static thread_local boost::asio::io_service service;

    SerialServiceImpl(const std::string& serialName)
        : serial{ service, serialName }
    {
    }

    ~SerialServiceImpl()
    {
        serial.close();
    }

    boost::asio::serial_port serial;
};

thread_local boost::asio::io_service SerialService::SerialServiceImpl::service;

SerialService::SerialService(std::string serialName)
    : implPtr{ std::make_unique<SerialServiceImpl>(serialName) }
{
}

SerialService::~SerialService() = default;

SerialService::SerialService(SerialService&& movingService) = default;
SerialService& SerialService::operator=(SerialService&& movingService) = default;


std::string SerialService::receive(size_t maximumBytesReceived)
{
    return Network::receive(implPtr->serial, maximumBytesReceived);
}

void SerialService::send(std::string data)
{
    Network::send(implPtr->serial, data);
}