#include "common/exception/error_codes.h"

namespace eurora::common {

struct ErrorMapping {
  ErrorCode code_;
  const char* message_;
};

constexpr ErrorMapping ErrorMessages[] = {
    {ErrorCode::kUnknownError, "An unknown error occurred"},
    {ErrorCode::kInvalidArgument, "Invalid argument provided"},
    {ErrorCode::kOutOfMemory, "Out of memory"},
    {ErrorCode::kFileNotFound, "File not found"},
    {ErrorCode::kAccessDenied, "Access denied"},
    {ErrorCode::kNetworkTimeout, "Network timeout"},
    {ErrorCode::kConnectionFailed, "Failed to establish a connection"},
    {ErrorCode::kDNSResolutionFailed, "Failed to resolve DNS"},
    {ErrorCode::kDatabaseConnectionFailed, "Failed to connect to the database"},
    {ErrorCode::kQueryExecutionFailed, "Failed to execute the database query"},
    {ErrorCode::kInvalidState, "Invalid state for this operation"},
    {ErrorCode::kOperationFailed, "The operation failed"},
    {ErrorCode::kNotImplemented, "This feature is not implemented"},
};

std::string GetErrorMessage(ErrorCode code) {
  for (const auto& mapping : ErrorMessages) {
    if (mapping.code_ == code) {
      return mapping.message_;
    }
  }
  return "";
}

}  // namespace eurora::common
