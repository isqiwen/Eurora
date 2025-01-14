#include "common/exception/exception.h"

#include <sstream>

namespace eurora::common {

std::string TrifilePath(const std::string& filePath) {
  size_t pos = filePath.find_last_of("/\\");
  return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
}

Exception::Exception(ErrorCode code, const std::string& description, const std::string& file, int line)
    : error_code_(code), description_(description), file_(TrifilePath(file)), line_(line) {}

const char* Exception::what() const noexcept {
  if (cached_what_.empty()) {
    std::ostringstream oss;
    oss << GetErrorMessage(error_code_);
    if (!description_.empty()) {
      oss << ": " << description_;
    }
    if (!file_.empty()) {
      oss << " (in " << file_ << ":" << line_ << ")";
    }
    cached_what_ = oss.str();
  }

  return cached_what_.c_str();
}

ErrorCode Exception::error_code() const noexcept {
  return error_code_;
}

const std::string& Exception::description() const noexcept {
  return description_;
}

}  // namespace eurora::common
