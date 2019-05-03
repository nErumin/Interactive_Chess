#include "TransmissionService.h"
#include "SocketConnection.h"
#include "SocketWrapper.h"
#include "ConnectionInternals.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>

using namespace Network;

TransmissionService::TransmissionService(SocketConnection& connection)
    : connectionRef{ connection }
{
}

std::string TransmissionService::receive(size_t maximumBytesReceived)
{
    try
    {
        std::vector<char> receivedData(maximumBytesReceived);
        auto& socket = ConnectionInternals::wrapper(connectionRef).socket();

        socket.read_some(boost::asio::buffer(receivedData));
        return { receivedData.begin(), receivedData.end() };
    }
    catch (const boost::system::system_error& error)
    {
        throw std::runtime_error{ "receiption failed" };
    }
}

void TransmissionService::send(std::string data)
{
    try
    {
        auto& socket = ConnectionInternals::wrapper(connectionRef).socket();

        boost::asio::write(socket, boost::asio::buffer(data, data.size()));
    }
    catch (const boost::system::system_error& error)
    {
        throw std::runtime_error{ "sending failed" };
    }
}