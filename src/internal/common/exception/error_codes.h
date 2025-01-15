#ifndef EURORA_COMMON_EXCEPTION_ERROR_CODES_H_
#define EURORA_COMMON_EXCEPTION_ERROR_CODES_H_

#pragma once

#include <string>

namespace eurora::common {

enum class ErrorCode {
    // System Errors (0x0000 - 0x00FF)
    kUnknownError    = 0x0000,  // An unknown error occurred
    kInvalidArgument = 0x0001,  // An invalid argument was provided
    kOutOfMemory     = 0x0002,  // Memory allocation failed
    kFileNotFound    = 0x0003,  // The specified file was not found
    kAccessDenied    = 0x0004,  // Permission denied to access the resource

    // Network Errors (0x0100 - 0x01FF)
    kNetworkTimeout      = 0x0100,  // The network operation timed out
    kConnectionFailed    = 0x0101,  // Failed to establish a network connection
    kDNSResolutionFailed = 0x0102,  // DNS resolution failed

    // Database Errors (0x0200 - 0x02FF)
    kDatabaseConnectionFailed = 0x0200,  // Failed to connect to the database
    kQueryExecutionFailed     = 0x0201,  // Execution of the database query failed

    // Application Logic Errors (0x0300 - 0x03FF)
    kInvalidState    = 0x0300,  // The operation is not valid in the current state
    kOperationFailed = 0x0301,  // The requested operation failed
    kNotImplemented  = 0x0302,  // The requested operation or feature is not implemented
};

std::string GetErrorMessage(ErrorCode code);

}  // namespace eurora::common

#endif  // EURORA_COMMON_EXCEPTION_ERROR_CODES_H_
