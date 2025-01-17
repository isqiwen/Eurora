#ifndef EURORA_UTILS_TIME_UTILS_H_
#define EURORA_UTILS_TIME_UTILS_H_

#pragma once

#include <ctime>
#include <string>

#include "eurora/utils/export_macros.h"

namespace eurora::utils {

class EURORA_API TimeUtils {
public:
    static std::string GetCurrentDateTimeStringInMS();
    static std::string GetTimestampString();
    static std::string GetTimestampWithMsString();
    static std::string GetCurrentDateString();
    static std::string GetCurrentTimeString(bool enable_ms = false);

private:
    static std::string FormatTime(const std::string& format, bool include_ms = false);
    static std::tm GetLocalTime(std::time_t time);
};

}  // namespace eurora::utils

#endif  // EURORA_UTILS_TIME_UTILS_H_
