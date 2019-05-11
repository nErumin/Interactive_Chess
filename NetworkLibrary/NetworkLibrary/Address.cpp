#include "Address.h"

using Network::Address;

Address::Address(Host hostName, Port portNumber)
    : endpoint{ std::make_pair(hostName, portNumber) }
{
}

auto Address::host() const -> Host
{
    return endpoint.first;
}

auto Address::port() const noexcept -> Port
{
    return endpoint.second;
}