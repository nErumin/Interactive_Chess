#pragma warning(disable: 4251)

#ifndef _NET_LIBRARY_SOCKET_CONNECTION_
#define _NET_LIBRARY_SOCKET_CONNECTION_

#include "LibraryAPI.h"
#include <memory>

namespace Network
{
    struct SocketWrapper;

    class NETWORK_API SocketConnection
    {
        friend struct ConnectionInternals;
    public:
        explicit SocketConnection(SocketWrapper wrapper);
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