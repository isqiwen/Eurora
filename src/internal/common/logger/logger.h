#ifndef EURORA_COMMON_LOGGER_LOGGER_H_
#define EURORA_COMMON_LOGGER_LOGGER_H_

#pragma once

#include "common/logger/wlog.hpp"

#define __FILENAME__ (eurora::common::Logger::GetShortname(__FILE__))

#define LOG(level, fmt, ...) eurora::common::Logger::Instance().Log({__FILENAME__, __LINE__, __FUNCTION__}, level, fmt, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) LOG(spdlog::level::trace, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(spdlog::level::debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(spdlog::level::info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(spdlog::level::warn, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(spdlog::level::err, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(spdlog::level::critical, fmt, ##__VA_ARGS__)

#define LOG_IF(condition, level, fmt, ...) \
  if (condition) {                         \
    LOG(level, fmt, ##__VA_ARGS__);        \
  }

#define STREAM_TRACE() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::trace, "")
#define STREAM_DEBUG() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::debug, "")
#define STREAM_INFO() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::info, "")
#define STREAM_WARN() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::warn, "")
#define STREAM_ERROR() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::err, "")
#define STREAM_FATAL() eurora::common::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::critical, "")

#endif  // EURORA_COMMON_LOGGER_LOGGER_H_
