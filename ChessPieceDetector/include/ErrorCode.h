#ifndef _NET_LIBRARY_ERROR_CODE_
#define _NET_LIBRARY_ERROR_CODE_

#include "LibraryAPI.h"

namespace Network
{
    enum class NETWORK_API ErrorCode
    {
        Disconnected = 1,
        ConnectFailed = 2,
        AcceptFailed = 4,
        SendFailed = 8,
        ReceiveFailed = 16,
        ListenFailed = 32,
        DisconnectFailed = 64
    };
}

#endif