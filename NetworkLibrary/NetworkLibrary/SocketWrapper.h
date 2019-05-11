#ifndef _NET_LIBRARY_SOCKET_WRAPPER_
#define _NET_LIBRARY_SOCKET_WRAPPER_

#include <memory>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include "SocketConnection.h"

namespace Network
{
    struct SocketWrapper final
    {
    public:
        using BaseSocket = boost::asio::ip::tcp::socket;
        using ServicePointer = std::shared_ptr<boost::asio::io_service>;
        
        explicit SocketWrapper(BaseSocket wrappingSocket, ServicePointer service);
        SocketWrapper(const SocketWrapper&) = delete;
        SocketWrapper(SocketWrapper&&);

        SocketWrapper& operator=(const SocketWrapper&) = delete;
        SocketWrapper& operator=(SocketWrapper&&);

        ~SocketWrapper();
    public:
        BaseSocket& socket() noexcept;
        const BaseSocket& socket() const noexcept;

        void disconnect();
    private:
        BaseSocket wrappedSocket;
        ServicePointer servicePtr;
    };
}
#endif
