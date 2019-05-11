#include "SocketWrapper.h"
#include "ErrorCode.h"
#include "NetworkError.h"
#include <iostream>

using Network::SocketWrapper;

SocketWrapper::SocketWrapper(BaseSocket wrappingSocket, ServicePointer service)
    : wrappedSocket{ std::move(wrappingSocket) },
      servicePtr{ service }
{
}

SocketWrapper::SocketWrapper(SocketWrapper&&) = default;

SocketWrapper& SocketWrapper::operator=(SocketWrapper&&) = default;

SocketWrapper::~SocketWrapper()
{
    try
    {
        if (socket().is_open())
        {
            disconnect();
        }
    }
    catch (...)
    {
        std::cerr << "Cannot disconnect while destructing... Is it OK?" << std::endl;
    }
}

auto SocketWrapper::socket() noexcept -> BaseSocket&
{
    return wrappedSocket;
}

auto SocketWrapper::socket() const noexcept -> const BaseSocket&
{
    return wrappedSocket;
}

void SocketWrapper::disconnect()
{
    using ShutdownType = boost::asio::socket_base::shutdown_type;
    
    try
    {
        socket().shutdown(ShutdownType::shutdown_both);
        socket().close();
    }
    catch (const boost::system::system_error& error)
    {
        throw NetworkError{ ErrorCode::DisconnectFailed, error.code().message() };
    }
}