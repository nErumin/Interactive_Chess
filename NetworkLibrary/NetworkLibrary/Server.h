#ifndef _NET_LIBRARY_SERVER_
#define _NET_LIBRARY_SERVER_

#include <memory>

namespace Network
{
    struct Address;
    class SocketConnection;

    class Server final
    {
    public:
        static constexpr int ListeningBackLog = 5;

        explicit Server(Address serverAddress);
        Server(const Server& copyingServer) = delete;
        Server(Server&& movingServer);

        Server& operator=(const Server& copyingServer) = delete;
        Server& operator=(Server&& movingServer);

        ~Server();
    public:
        SocketConnection waitClient() const;
    private:
        class ServerImpl;
        std::unique_ptr<ServerImpl> implPtr;
    };
}

#endif