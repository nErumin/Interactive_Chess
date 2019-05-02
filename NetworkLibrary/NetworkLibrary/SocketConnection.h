#ifndef _NET_LIBRARY_SOCKET_CONNECTION_
#define _NET_LIBRARY_SOCKET_CONNECTION_

#include <memory>

namespace Network
{
    struct SocketWrapper;

    class SocketConnection
    {
        friend struct ConnectionInternals;
    public:
        SocketConnection(SocketWrapper wrapper);
        SocketConnection(const SocketConnection& copyingConnection) = delete;
        SocketConnection(SocketConnection&& movingConnection);

        SocketConnection& operator=(const SocketConnection& copyingConnection) = delete;
        SocketConnection& operator=(SocketConnection&& movingConnection);

        ~SocketConnection();
    public:
        void disconnect();
    private:
        std::unique_ptr<SocketWrapper> socketWrapper;
    };
}

#endif