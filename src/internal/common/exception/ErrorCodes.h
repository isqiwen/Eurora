#ifndef EURORA_COMMON_EXCEPTION_ERROR_CODES_
#define EURORA_COMMON_EXCEPTION_ERROR_CODES_

#pragma once

#include <string>


namespace eurora::common {

    enum class ErrorCode {
        // System Errors (0x0000 - 0x00FF)
        UnknownError        = 0x0000, // An unknown error occurred
        InvalidArgument     = 0x0001, // An invalid argument was provided
        OutOfMemory         = 0x0002, // Memory allocation failed
        FileNotFound        = 0x0003, // The specified file was not found
        AccessDenied        = 0x0004, // Permission denied to access the resource

        // Network Errors (0x0100 - 0x01FF)
        NetworkTimeout      = 0x0100, // The network operation timed out
        ConnectionFailed    = 0x0101, // Failed to establish a network connection
        DNSResolutionFailed = 0x0102, // DNS resolution failed

        // Database Errors (0x0200 - 0x02FF)
        DatabaseConnectionFailed = 0x0200, // Failed to connect to the database
        QueryExecutionFailed     = 0x0201, // Execution of the database query failed

        // Application Logic Errors (0x0300 - 0x03FF)
        InvalidState       = 0x0300, // The operation is not valid in the current state
        OperationFailed    = 0x0301, // The requested operation failed
        NotImplemented     = 0x0302, // The requested operation or feature is not implemented
    };

    std::string getErrorMessage(ErrorCode code);

} // namespace eurora::common

#endif // EURORA_COMMON_EXCEPTION_ERROR_CODES_
