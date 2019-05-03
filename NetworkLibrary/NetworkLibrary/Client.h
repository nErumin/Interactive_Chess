#ifndef _NET_LIBRARY_CLIENT_
#define _NET_LIBRARY_CLIENT_

#include <memory>

namespace Network
{
    struct Address;
    class SocketConnection;

    class Client final
    {
    public:
        explicit Client(Address serverAddress);
        Client(const Client& copyingClient) = delete;
        Client(Client&& movingClient);

        Client& operator=(const Client& copyingClient) = delete;
        Client& operator=(Client&& movingClient);

        ~Client();
    public:
        SocketConnection connect();
    private:
        class ClientImpl;
        std::unique_ptr<ClientImpl> implPtr;
    };
}

#endif