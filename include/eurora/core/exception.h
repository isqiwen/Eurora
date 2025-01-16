#ifndef EURORA_UTILS_EXCEPTION_EXCEPTION_
#define EURORA_UTILS_EXCEPTION_EXCEPTION_

#pragma once

#include <exception>

#include "error_codes.h"

namespace eurora::utils {

class Exception : public std::exception {
public:
    Exception(ErrorCode code, const std::string& description = "", const std::string& file = "", int line = 0);

    const char* what() const noexcept override;

    ErrorCode error_code() const noexcept;

    const std::string& description() const noexcept;

private:
    ErrorCode error_code_;
    std::string description_;
    std::string file_;
    int line_;
    mutable std::string cached_what_;
};

}  // namespace eurora::utils

#define EURORA_THROW_EXCEPTION(code, desc) throw eurora::utils::Exception(code, desc, __FILE__, __LINE__)

#define EURORA_THROW_SIMPLE_EXCEPTION(exception_name, desc) throw exception_name(desc, __FILE__, __LINE__)

#define EURORA_MAKE_SIMPLE_EXCEPTION(exception_name)                                                                                              \
    class exception_name : public eurora::utils::Exception {                                                                                      \
    public:                                                                                                                                       \
        exception_name(const std::string& description = "", const std::string& file = __FILE__, int line = __LINE__)                              \
            : eurora::utils::Exception(eurora::utils::ErrorCode::kUnknownError, std::string(#exception_name) + ": " + description, file, line) {} \
    };

#define EURORA_MAKE_SIMPLE_EXCEPTION_WITH_CODE(exception_name, error_code)                                           \
    class exception_name : public eurora::utils::Exception {                                                         \
    public:                                                                                                          \
        exception_name(const std::string& description = "", const std::string& file = __FILE__, int line = __LINE__) \
            : eurora::utils::Exception(error_code, std::string(#exception_name) + ": " + description, file, line) {} \
    };

#endif  // EURORA_UTILS_EXCEPTION_EXCEPTION_
