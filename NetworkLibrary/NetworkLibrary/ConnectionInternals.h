#ifndef _NET_LIBRARY_CONNECTION_INTERNALS_
#define _NET_LIBRARY_CONNECTION_INTERNALS_

#include "SocketConnection.h"

namespace Network
{
    struct SocketWrapper;

    struct ConnectionInternals final
    {
    public:
        static SocketWrapper& wrapper(SocketConnection& connection)
        {
            return *connection.socketWrapper;
        }
    };
}

#endif