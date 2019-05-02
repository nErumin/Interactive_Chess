#include "SocketConnection.h"
#include "SocketWrapper.h"

using Network::SocketConnection;
using Network::SocketWrapper;

SocketConnection::SocketConnection(SocketWrapper wrapper)
    : socketWrapper{ std::make_unique<SocketWrapper>(std::move(wrapper)) }
{
}

SocketConnection::SocketConnection(SocketConnection&& movingWrapper) = default;
SocketConnection& SocketConnection::operator=(SocketConnection&& movingWrapper) = default;

SocketConnection::~SocketConnection() = default;

void SocketConnection::disconnect()
{
    using ShutdownType = boost::asio::socket_base::shutdown_type;

    try
    {
        socketWrapper->socket().shutdown(ShutdownType::shutdown_both);
        socketWrapper->socket().close();
    }
    catch (const boost::system::system_error&)
    {
        throw std::runtime_error{ "disconnection failed" };
    }
}
