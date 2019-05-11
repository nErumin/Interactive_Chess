#include "NetworkError.h"
#include "ErrorCode.h"

using Network::NetworkError;
using Network::ErrorCode;

NetworkError::NetworkError(ErrorCode code, std::string message)
    : std::runtime_error{ message },
      errorCode{ code }
{
}

ErrorCode NetworkError::code() const noexcept
{
    return errorCode;
}