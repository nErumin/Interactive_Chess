#pragma warning(disable: 4251)

#ifndef _NET_LIBRARY_SERIAL_SERVICE_
#define _NET_LIBRARY_SERIAL_SERVICE_

#include "LibraryAPI.h"
#include <string>
#include <memory>

namespace Network
{
    class NETWORK_API SerialService
    {
    public:
        explicit SerialService(std::string serialName);
        SerialService(const SerialService& copyingService) = delete;
        SerialService(SerialService&& movingService);

        SerialService& operator=(const SerialService& copyingService) = delete;
        SerialService& operator=(SerialService&& movingService);

        ~SerialService();
    public:
        std::string receive(size_t maximumBytesReceived);
        void send(std::string data);
    private:
        class SerialServiceImpl;
        std::unique_ptr<SerialServiceImpl> implPtr;
    };
}

#endif