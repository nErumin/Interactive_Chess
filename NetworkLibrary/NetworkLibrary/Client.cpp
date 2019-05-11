#include "Client.h"
#include "Address.h"
#include "SocketConnection.h"
#include "SocketWrapper.h"
#include "ErrorCode.h"
#include "NetworkError.h"

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>

using namespace Network;
using boost::asio::ip::tcp;

class Client::ClientImpl
{
public:
    explicit ClientImpl(Address serverAddress)
        : servicePtr{ std::make_shared<boost::asio::io_service>() },
          targetAddress{ std::move(serverAddress) }
    {
    }
public:
    SocketConnection connect()
    {
        try
        {
            tcp::socket clientSocket{ *servicePtr };
            auto resolutionIterator = resolveAddress(targetAddress);
            boost::asio::connect(clientSocket, resolutionIterator);

            return SocketConnection{ SocketWrapper(std::move(clientSocket), servicePtr) };
        }
        catch (const boost::system::system_error& error)
        {
            throw NetworkError{ ErrorCode::ConnectFailed, error.code().message() };
        }
    }
private:
    tcp::resolver::iterator resolveAddress(Address serverAddress)
    {
        tcp::resolver resolver{ *servicePtr };
        tcp::resolver::query query{ serverAddress.host(), std::to_string(serverAddress.port()) };

        return resolver.resolve(query);
    }
private:
    std::shared_ptr<boost::asio::io_service> servicePtr;
    Address targetAddress;
};

Client::Client(Address serverAddress)
    : implPtr{ std::make_unique<ClientImpl>(std::move(serverAddress)) }
{

}

SocketConnection Client::connect()
{
    return implPtr->connect();
}

Client::Client(Client&& movingClient) = default;
Client& Client::operator=(Client&& movingClient) = default;

Client::~Client()= default;