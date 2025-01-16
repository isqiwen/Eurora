#ifndef EURORA_UTILS_LOGGER_WLOG_
#define EURORA_UTILS_LOGGER_WLOG_

#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>

#include <fmt/Printf.h>
#include <spdlog/async.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>

#include "utils/pattern/singleton.hpp"
#include "utils/time_utils.h"

/// spdlog wrap class

namespace eurora::utils {

class Logger final : public Singleton<Logger> {
    friend class Singleton<Logger>;

public:
    /// let logger like stream
    struct LogStream : public std::ostringstream {
    public:
        LogStream(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, std::string_view prefix) : loc_(loc), lvl_(lvl), prefix_(prefix) {}

        ~LogStream() { Flush(); }

        void Flush() { Logger::Instance().Log(loc_, lvl_, (prefix_ + str()).c_str()); }

    private:
        spdlog::source_loc loc_;
        spdlog::level::level_enum lvl_ = spdlog::level::info;
        std::string prefix_;
    };

private:
    Logger(Token) {}

    ~Logger() { Shutdown(); }

public:
    bool InitFromConfig(const std::string& config_file);

    void Shutdown() {
        spdlog::shutdown();
        initialized_ = false;
    }

    template <typename... Args>
    void Log(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args&... args) {
        spdlog::log(loc, lvl, fmt, args...);
    }

    template <typename... Args>
    void Printf(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args&... args) {
        spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
    }

    spdlog::level::level_enum Level() { return log_level_; }

    void SetLevel(unsigned lvl) {
        log_level_ = static_cast<spdlog::level::level_enum>(lvl);
        spdlog::set_level(log_level_);
    }

    void FlushOn(spdlog::level::level_enum lvl) { spdlog::flush_on(lvl); }

    static const char* GetShortname(std::string_view path);

private:
    std::atomic_bool initialized_        = false;
    spdlog::level::level_enum log_level_ = spdlog::level::trace;
};

}  // namespace eurora::utils

#endif  // EURORA_UTILS_LOGGER_WLOG_
