#ifndef _NET_LIBRARY_SOCKET_WRAPPER_
#define _NET_LIBRARY_SOCKET_WRAPPER_

#include <boost/asio/ip/tcp.hpp>
#include "SocketConnection.h"

namespace Network
{
    struct SocketWrapper final
    {
    public:
        using BaseSocket = boost::asio::ip::tcp::socket;
        
        explicit SocketWrapper(BaseSocket wrappingSocket);
    public:
        BaseSocket& socket() noexcept;
        const BaseSocket& socket() const noexcept;
    private:
        BaseSocket wrappedSocket;
    };
}
#endif
