#include "TransmissionService.h"
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
    try
    {
        std::string receivedData(maximumBytesReceived, (char) 0);
        auto& socket = ConnectionInternals::wrapper(connectionRef).socket();
        
        size_t receivedLength = socket.read_some(boost::asio::buffer(receivedData));
        receivedData.resize(receivedLength);

        return { receivedData.begin(), receivedData.end() };
    }
    catch (const boost::system::system_error& error)
    {
        ErrorCode code = error.code() == boost::asio::error::eof ?
                         ErrorCode::Disconnected :
                         ErrorCode::ReceiveFailed;

        throw NetworkError{ code, error.code().message() };
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
        ErrorCode code = error.code() == boost::asio::error::eof ?
                         ErrorCode::Disconnected :
                         ErrorCode::SendFailed;

        throw NetworkError{ code, error.code().message() };
    }
}