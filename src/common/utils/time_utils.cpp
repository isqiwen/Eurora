#include "common/utils/time_utils.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace eurora::common {

std::string TimeUtils::FormatTime(const std::string& format, bool include_ms) {
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  auto millisec = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

  std::ostringstream ss;
  auto local_now_time = GetLocalTime(now_time);
  ss << std::put_time(&local_now_time, format.c_str());
  if (include_ms) {
    ss << '.' << std::setw(3) << std::setfill('0') << (millisec % 1000);
  }
  return ss.str();
}

std::tm TimeUtils::GetLocalTime(std::time_t time) {
  std::tm local_tm;
#if defined(_WIN32) || defined(_WIN64)
  localtime_s(&local_tm, &time);
#else
  localtime_r(&time, &local_tm);
#endif
  return local_tm;
}

std::string TimeUtils::GetCurrentDateTimeStringInMS() {
  return FormatTime("%Y_%m_%d-%H:%M:%S", true);
}

std::string TimeUtils::GetTimestampString() {
  return FormatTime("%Y%m%d%H%M%S", false);
}

std::string TimeUtils::GetTimestampWithMsString() {
  return FormatTime("%Y%m%d%H%M%S", true);
}

std::string TimeUtils::GetCurrentDateString() {
  return FormatTime("%Y-%m-%d", false);
}

std::string TimeUtils::GetCurrentTimeString(bool enable_ms) {
  return FormatTime("%H:%M:%S", enable_ms);
}

}  // namespace eurora::common
