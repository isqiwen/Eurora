#ifndef EURORA_COMMON_EXCEPTION_EXCEPTION_
#define EURORA_COMMON_EXCEPTION_EXCEPTION_

#pragma once

#include <exception>

#include "ErrorCodes.h"

namespace eurora::common {

class Exception : public std::exception
{
  public:
    Exception(ErrorCode code, const std::string &description = "", const std::string &file = "", int line = 0);

    const char *what() const noexcept override;

    ErrorCode code() const noexcept;

    const std::string &description() const noexcept;

  private:
    ErrorCode mErrorCode;
    std::string mDescription;
    std::string mFile;
    int mLine;
    mutable std::string mCachedWhat;
};

}  // namespace eurora::common

#define EURORA_THROW_EXCEPTION(code, desc) throw eurora::common::Exception(code, desc, __FILE__, __LINE__)

#define EURORA_THROW_SIMPLE_EXCEPTION(exceptionName, desc) throw exceptionName(desc, __FILE__, __LINE__)

#define EURORA_MAKE_SIMPLE_EXCEPTION(exceptionName)                                                                                          \
    class exceptionName : public eurora::common::Exception                                                                                   \
    {                                                                                                                                        \
      public:                                                                                                                                \
        exceptionName(const std::string &description = "", const std::string &file = __FILE__, int line = __LINE__)                          \
          : eurora::common::Exception(eurora::common::ErrorCode::UnknownError, std::string(#exceptionName) + ": " + description, file, line) \
        {}                                                                                                                                   \
    };

#define EURORA_MAKE_SIMPLE_EXCEPTION_WITH_CODE(exceptionName, errorCode)                                            \
    class exceptionName : public eurora::common::Exception                                                          \
    {                                                                                                               \
      public:                                                                                                       \
        exceptionName(const std::string &description = "", const std::string &file = __FILE__, int line = __LINE__) \
          : eurora::common::Exception(errorCode, std::string(#exceptionName) + ": " + description, file, line)      \
        {}                                                                                                          \
    };

#endif  // EURORA_COMMON_EXCEPTION_EXCEPTION_
