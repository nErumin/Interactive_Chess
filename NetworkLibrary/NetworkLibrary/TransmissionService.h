#ifndef _NET_LIBRARY_TRANSPORT_SERVICE_
#define _NET_LIBRARY_TRANSPORT_SERVICE_

#include "LibraryAPI.h"
#include <string>
#include <memory>

namespace Network
{
    class SocketConnection;

    class NETWORK_API TransmissionService
    {
    public:
        explicit TransmissionService(SocketConnection& connection);
        std::string receive(size_t maximumBytesReceived);
        void send(std::string data);
    private:
        SocketConnection& connectionRef;
    };
}

#endif