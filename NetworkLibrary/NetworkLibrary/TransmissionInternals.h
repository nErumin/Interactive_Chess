#ifndef _NET_LIBRARY_TRANSMISSION_INTERNALS_
#define _NET_LIBRARY_TRANSMISSION_INTERNALS_

#include "SocketConnection.h"
#include "NetworkError.h"
#include "ErrorCode.h"

#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>

namespace Network
{
    template <typename Stream>
    std::string receive(Stream& streamRef, size_t maximumBytesReceived)
    {
        try
        {
            std::string receivedData(maximumBytesReceived, (char) 0);
            size_t receivedLength = streamRef.read_some(boost::asio::buffer(receivedData));
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

    template <typename Stream>
    void send(Stream& streamRef, std::string data)
    {
        try
        {
            boost::asio::write(streamRef, boost::asio::buffer(data, data.size()));
        }
        catch (const boost::system::system_error & error)
        {
            ErrorCode code = error.code() == boost::asio::error::eof ?
                ErrorCode::Disconnected :
                ErrorCode::SendFailed;

            throw NetworkError{ code, error.code().message() };
        }
    }
}

#endif