#include "Server.h"
#include "Address.h"
#include "SocketConnection.h"
#include "SocketWrapper.h"

#include <boost/asio/placeholders.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread/future.hpp>

using namespace Network;

using boost::asio::ip::tcp;
using boost::asio::io_service;

class Server::ServerImpl
{
private:
    using AcceptancePromise = boost::promise<SocketConnection>;

public:
    explicit ServerImpl(Address serverAddress)
        : ServerImpl{ std::move(serverAddress), 1 }
    {
    }

    ServerImpl(Address serverAddress, int clientBackLog)
        : service{},
          clientListener{ service, tcp::endpoint(tcp::v4(), serverAddress.port()) }
    {
        try
        {
            clientListener.listen(clientBackLog);
        }
        catch (const boost::system::system_error&)
        {
            throw std::runtime_error{ "listening failed" };
        }
    }

    boost::unique_future<SocketConnection> startAccept()
    {
        auto acceptedSocketPtr = std::make_shared<tcp::socket>(service);
        auto promise = std::make_shared<AcceptancePromise>();

        auto acceptanceHandler = 
            std::bind(&ServerImpl::acceptClient, this, promise, acceptedSocketPtr, std::placeholders::_1);
        
        clientListener.async_accept(*acceptedSocketPtr, std::move(acceptanceHandler));

        return promise->get_future();
    }

    void runTask()
    {
        service.run();
        service.reset();
    }
private:
    void acceptClient(std::shared_ptr<AcceptancePromise> promise,
                      std::shared_ptr<tcp::socket> socketPtr, 
                      const boost::system::error_code& errorCode)
    {
        if (errorCode.failed())
        {
            auto exceptionPtr = std::make_exception_ptr(std::runtime_error("acceptance failed"));
            promise->set_exception(exceptionPtr);
        }

        SocketConnection newConnection{ SocketWrapper(std::move(*socketPtr)) };

        promise->set_value(std::move(newConnection));
    }

private:
    io_service service;
    tcp::acceptor clientListener;
};

Server::Server(Address serverAddress)
    : implPtr{ std::make_unique<ServerImpl>(std::move(serverAddress), ListeningBackLog) }
{

}

Server::~Server() = default;

Server::Server(Server&& movingServer) = default;
Server& Server::operator=(Server&& movingServer) = default;

SocketConnection Server::waitClient() const
{
    auto acceptanceFuture = implPtr->startAccept();

    implPtr->runTask();
    return acceptanceFuture.get();
}