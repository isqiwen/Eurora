#include "common/exception/ErrorCodes.h"

namespace eurora::common {

struct ErrorMapping {
    ErrorCode mCode;
    const char *mMessage;
};

constexpr ErrorMapping ErrorMessages[] = {
    { ErrorCode::UnknownError, "An unknown error occurred" },
    { ErrorCode::InvalidArgument, "Invalid argument provided" },
    { ErrorCode::OutOfMemory, "Out of memory" },
    { ErrorCode::FileNotFound, "File not found" },
    { ErrorCode::AccessDenied, "Access denied" },
    { ErrorCode::NetworkTimeout, "Network timeout" },
    { ErrorCode::ConnectionFailed, "Failed to establish a connection" },
    { ErrorCode::DNSResolutionFailed, "Failed to resolve DNS" },
    { ErrorCode::DatabaseConnectionFailed, "Failed to connect to the database" },
    { ErrorCode::QueryExecutionFailed, "Failed to execute the database query" },
    { ErrorCode::InvalidState, "Invalid state for this operation" },
    { ErrorCode::OperationFailed, "The operation failed" },
    { ErrorCode::NotImplemented, "This feature is not implemented" },
};

std::string getErrorMessage(ErrorCode code)
{
    for (const auto &mapping : ErrorMessages)
    {
        if (mapping.mCode == code)
        {
            return mapping.mMessage;
        }
    }
    return "";
}

}  // namespace eurora::common
