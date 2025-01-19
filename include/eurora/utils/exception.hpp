#pragma once

#include <exception>
#include <sstream>
#include <string>
#include <filesystem>

#include "eurora/utils/error_codes.h"

namespace eurora::utils {

class Exception : public std::exception {
public:
    Exception(ErrorCode code, const std::string& description = "", const std::string& file = "", int line = 0)
        : error_code_(code), description_(description), file_(std::filesystem::path(file).filename().string()), line_(line) {}

    const char* what() const noexcept override {
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

    ErrorCode error_code() const noexcept { return error_code_; }

    const std::string& description() const noexcept { return description_; }

private:
    ErrorCode error_code_;
    std::string description_;
    std::string file_;
    int line_;
    mutable std::string cached_what_;
};

#define EURORA_THROW_ERROR(code, desc) throw eurora::utils::Exception(code, desc, __FILE__, __LINE__)

#define EURORA_THROW_EXCEPTION(exception_type, desc) throw exception_type(desc, __FILE__, __LINE__)

#define EURORA_DEFINE_EXCEPTION(exception_name)                                                                                                   \
    class exception_name : public eurora::utils::Exception {                                                                                      \
    public:                                                                                                                                       \
        exception_name(const std::string& description = "", const std::string& file = __FILE__, int line = __LINE__)                              \
            : eurora::utils::Exception(eurora::utils::ErrorCode::kUnknownError, std::string(#exception_name) + ": " + description, file, line) {} \
    };

#define EURORA_DEFINE_EXCEPTION_WITH_CODE(exception_name, error_code)                                                \
    class exception_name : public eurora::utils::Exception {                                                         \
    public:                                                                                                          \
        exception_name(const std::string& description = "", const std::string& file = __FILE__, int line = __LINE__) \
            : eurora::utils::Exception(error_code, std::string(#exception_name) + ": " + description, file, line) {} \
    };

EURORA_DEFINE_EXCEPTION_WITH_CODE(InvalidArgumentException, eurora::utils::ErrorCode::kInvalidArgument)
EURORA_DEFINE_EXCEPTION_WITH_CODE(FileNotFoundException, eurora::utils::ErrorCode::kIO_FileNotFound)

}  // namespace eurora::utils
