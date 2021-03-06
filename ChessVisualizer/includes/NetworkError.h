#pragma warning(disable: 4275)

#ifndef _NET_LIBRARY_NETWORK_ERROR_
#define _NET_LIBRARY_NETWORK_ERROR_

#include "LibraryAPI.h"
#include <stdexcept>

namespace Network
{
    enum class ErrorCode;

    class NETWORK_API NetworkError : public std::runtime_error
    {
    public:
        NetworkError(ErrorCode code, std::string message);
        ErrorCode code() const noexcept;
    private:
        ErrorCode errorCode;    
    };
}

#endif