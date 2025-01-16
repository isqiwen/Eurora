#ifndef EURORA_UTILS_LOGGER_LOGGER_H_
#define EURORA_UTILS_LOGGER_LOGGER_H_

#pragma once

#include "wlog.hpp"

#define __FILENAME__ (eurora::utils::Logger::GetShortname(__FILE__))

#define LOG(level, fmt, ...) eurora::utils::Logger::Instance().Log({__FILENAME__, __LINE__, __FUNCTION__}, level, fmt, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) LOG(spdlog::level::trace, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(spdlog::level::debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(spdlog::level::info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(spdlog::level::warn, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(spdlog::level::err, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(spdlog::level::critical, fmt, ##__VA_ARGS__)

#define LOG_IF(condition, level, fmt, ...) \
    if (condition) {                       \
        LOG(level, fmt, ##__VA_ARGS__);    \
    }

#define STREAM_TRACE() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::trace, "")
#define STREAM_DEBUG() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::debug, "")
#define STREAM_INFO() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::info, "")
#define STREAM_WARN() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::warn, "")
#define STREAM_ERROR() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::err, "")
#define STREAM_FATAL() eurora::utils::Logger::LogStream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::critical, "")

#endif  // EURORA_UTILS_LOGGER_LOGGER_H_
