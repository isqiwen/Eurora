#ifndef EURORA_COMMON_UTILS_TIME_UTILS_H_
#define EURORA_COMMON_UTILS_TIME_UTILS_H_

#pragma once

#include <ctime>
#include <string>

namespace eurora::common {

class TimeUtils {
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

}  // namespace eurora::common

#endif  // EURORA_COMMON_UTILS_TIME_UTILS_H_
