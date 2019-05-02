#include "SocketWrapper.h"

using Network::SocketWrapper;

SocketWrapper::SocketWrapper(BaseSocket wrappingSocket)
    : wrappedSocket{ std::move(wrappingSocket) }
{
}

auto SocketWrapper::socket() noexcept -> BaseSocket&
{
    return wrappedSocket;
}

auto SocketWrapper::socket() const noexcept -> const BaseSocket&
{
    return wrappedSocket;
}