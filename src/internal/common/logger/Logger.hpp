#ifndef EURORA_COMMON_LOGGER_LOGGER_H_
#define EURORA_COMMON_LOGGER_LOGGER_H_

#pragma once

#include "common/logger/WLog.hpp"

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5
#define LOG_LEVEL_CLOSE 6

#define __FILENAME__ (eurora::common::Logger::getShortname(__FILE__))

#if (LOGGER_LEVEL <= LOG_LEVEL_TRACE)
#define LOG_TRACE(fmt, ...) \
    eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::trace, fmt, ##__VA_ARGS__);
#define PRINT_TRACE(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::trace, fmt, ##__VA_ARGS__);
#define STREAM_TRACE() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::trace, "")
#else
#define LOG_TRACE(fmt, ...)
#define PRINT_TRACE(fmt, ...)
#define STREAM_TRACE() eurora::common::LoggerNone::instance()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_DEBUG)
#define LOG_DEBUG(fmt, ...) \
    eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::debug, fmt, ##__VA_ARGS__);
#define PRINT_DEBUG(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::debug, fmt, ##__VA_ARGS__);
#define STREAM_DEBUG() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::debug, "")
#else
#define LOG_DEBUG(fmt, ...)
#define PRINT_DEBUG(fmt, ...)
#define STREAM_DEBUG() eurora::common::LoggerNone::instance()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_INFO)
#define LOG_INFO(fmt, ...) eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::info, fmt, ##__VA_ARGS__);
#define PRINT_INFO(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::info, fmt, ##__VA_ARGS__);
#define STREAM_INFO() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::info, "")
#else
#define LOG_INFO(fmt, ...)
#define PRINT_INFO(fmt, ...)
#define STREAM_INFO() eurora::common::LoggerNone::instance()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_WARN)
#define LOG_WARN(fmt, ...) eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::warn, fmt, ##__VA_ARGS__);
#define PRINT_WARN(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::warn, fmt, ##__VA_ARGS__);
#define STREAM_WARN() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::warn, "")
#else
#define LOG_WARN(fmt, ...)
#define PRINT_WARN(fmt, ...)
#define STREAM_WARN() eurora::common::LoggerNone::instance()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_ERROR)
#define LOG_ERROR(fmt, ...) \
    eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::error, fmt, ##__VA_ARGS__);
#define PRINT_ERROR(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::error, fmt, ##__VA_ARGS__);
#define STREAM_ERROR() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::error, "")
#else
#define LOG_ERROR(fmt, ...)
#define PRINT_ERROR(fmt, ...)
#define STREAM_ERROR() eurora::common::LoggerNone::instance()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_FATAL)
#define LOG_FATAL(fmt, ...) \
    eurora::common::Logger::instance().log({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::fatal, fmt, ##__VA_ARGS__);
#define PRINT_FATAL(fmt, ...) \
    eurora::common::Logger::instance().printf({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::fatal, fmt, ##__VA_ARGS__);
#define STREAM_FATAL() eurora::common::Logger::LogStream({ __FILENAME__, __LINE__, __FUNCTION__ }, spdlog::level::fatal, "")
#else
#define LOG_FATAL(fmt, ...)
#define PRINT_FATAL(fmt, ...)
#define STREAM_FATAL() eurora::common::LoggerNone::instance()
#endif

#endif  // EURORA_COMMON_LOGGER_LOGGER_H_
