#ifndef EURORA_COMMON_LOGGER_WLOG_
#define EURORA_COMMON_LOGGER_WLOG_

#pragma once

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

#include <filesystem>
#include <memory>
#include <sstream>

#include "common/pattern/Singleton.hpp"
#include "common/utils/time_utils.h"

/// spdlog wrap class

namespace eurora::common {

class Logger final : public Singleton<Logger> {
  friend class Singleton<Logger>;

public:
  /// let logger like stream
  struct LogStream : public std::ostringstream {
  public:
    LogStream(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, std::string_view prefix) : loc_(loc), lvl_(lvl), prefix_(prefix) {}

    ~LogStream() { flush(); }

    void flush() { Logger::Instance().Log(loc_, lvl_, (prefix_ + str()).c_str()); }

  private:
    spdlog::source_loc loc_;
    spdlog::level::level_enum lvl_ = spdlog::level::info;
    std::string prefix_;
  };

private:
  Logger(Token) {}

  ~Logger() { Shutdown(); }

public:
  bool Init(std::string_view log_file_path) {
    namespace fs = std::filesystem;

    if (initialized_) {
      return true;
    }

    try {
      // check log path and try to create log directory
      fs::path log_path(log_file_path);
      fs::path log_dir      = log_path.parent_path();
      std::string base_name = log_path.stem().string();
      std::string extension = log_path.extension().string();

      if (!fs::exists(log_dir)) {
        fs::create_directories(log_dir);
      }

      // Generate a unique log file name for the same dayS
      std::string date_str = common::TimeUtils::GetCurrentDateString();

      int count = 0;
      fs::path unique_log_file;
      do {
        std::stringstream ss;
        ss << base_name << "_" << date_str << (count >= 0 ? "_" + std::to_string(count) : "") << extension;
        unique_log_file = log_dir / ss.str();
        count++;
      } while (fs::exists(unique_log_file));

      // initialize spdlog
      constexpr std::size_t log_buffer_size     = 32 * 1024;         // 32Kb
      constexpr std::size_t max_file_size       = 50 * 1024 * 1024;  // 50M
      constexpr std::size_t max_number_of_files = 100;
      spdlog::init_thread_pool(log_buffer_size, 1);
      std::vector<spdlog::sink_ptr> sinks;

      auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(unique_log_file.string(), max_file_size, max_number_of_files);
      sinks.push_back(file_sink);

#if defined(_DEBUG) && defined(_WIN32) && !defined(NO_CONSOLE_LOG)
      auto ms_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
      sinks.push_back(ms_sink);
#endif  //  _DEBUG

#if !defined(_WIN32) && !defined(NO_CONSOLE_LOG)
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      sinks.push_back(console_sink);
#endif
      spdlog::set_default_logger(std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end()));

      spdlog::set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
      spdlog::flush_on(spdlog::level::warn);
      spdlog::set_level(log_level_);
    } catch (const std::exception& e) {
      spdlog::error("Failed to initialize logger: {}", e.what());
      assert(false);
      return false;
    }
    initialized_ = true;

    return true;
  }

  void Shutdown() { spdlog::shutdown(); }

  template <typename... Args>
  void Log(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args&... args) {
    spdlog::log(loc, lvl, fmt, args...);
  }

  template <typename... Args>
  void Printf(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args&... args) {
    spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
  }

  spdlog::level::level_enum level() { return log_level_; }

  void set_level(unsigned lvl) {
    log_level_ = static_cast<spdlog::level::level_enum>(lvl);
    spdlog::set_level(log_level_);
  }

  void SetFlushOn(spdlog::level::level_enum lvl) { spdlog::flush_on(lvl); }

  static const char* GetShortname(std::string_view path) {
    if (path.empty()) {
      return path.data();
    }

    size_t pos = path.find_last_of("/\\");
    return path.data() + ((pos == path.npos) ? 0 : pos + 1);
  }

private:
  std::atomic_bool initialized_        = false;
  spdlog::level::level_enum log_level_ = spdlog::level::trace;
};

class LoggerNone final : public Singleton<LoggerNone> {
  friend class Singleton<LoggerNone>;

private:
  LoggerNone(Token) {}

  ~LoggerNone() = default;

  LoggerNone& operator<<(const char* content) { return *this; }
};

}  // namespace eurora::common

#endif  // EURORA_COMMON_LOGGER_WLOG_
