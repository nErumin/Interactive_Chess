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
    socketWrapper->disconnect();
}
