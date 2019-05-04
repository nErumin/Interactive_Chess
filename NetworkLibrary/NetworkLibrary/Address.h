#pragma warning(disable: 4251)

#ifndef _NET_LIBRARY_ADDRESS_
#define _NET_LIBRARY_ADDRESS_

#include "LibraryAPI.h"
#include <string>
#include <utility>

namespace Network
{
    struct NETWORK_API Address final
    {
    public:
        using Host = std::string;
        using Port = unsigned short;
        
        Address(Host hostName, Port portNumber);
        
        Host host() const;
        Port port() const noexcept;
    private:
        const std::pair<Host, Port> endpoint;
    };
}
#endif
