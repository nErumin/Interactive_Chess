#include "TransmissionService.h"
#include "TransmissionInternals.h"
#include "SocketConnection.h"
#include "SocketWrapper.h"
#include "ConnectionInternals.h"
#include "ErrorCode.h"
#include "NetworkError.h"

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
    auto& socket = ConnectionInternals::wrapper(connectionRef).socket();

    return Network::receive(socket, maximumBytesReceived);
}

void TransmissionService::send(std::string data)
{
    auto& socket = ConnectionInternals::wrapper(connectionRef).socket();
 
    Network::send(socket, data);
}