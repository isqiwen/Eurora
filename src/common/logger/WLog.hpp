#ifndef EURORA_COMMON_LOGGER_WLOG_
#define EURORA_COMMON_LOGGER_WLOG_

#pragma once

#include <filesystem>
#include <sstream>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "common/pattern/Singleton.hpp"


/// spdlog wrap class

namespace eurora::common {

    class Logger final : public Singleton<Logger> {
        friend class Singleton<Logger>;
    public:
        /// let logger like stream
        struct LogStream : public std::ostringstream
        {
        public:
            LogStream(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, std::string_view prefix)
                : mLoc(loc)
                , mLvl(lvl)
                , mPrefix(prefix)
            {
            }

            ~LogStream()
            {
                flush();
            }

            void flush()
            {
                Logger::instance().log(mLoc, mLvl, (mPrefix + str()).c_str());
            }

        private:
            spdlog::source_loc          mLoc;
            spdlog::level::level_enum   mLvl = spdlog::level::info;
            std::string                 mPrefix;
        };

    private:
        Logger(token) {}

        ~Logger() { shutDown(); }

    public:
        bool init(std::string_view logFilePath) {
            namespace fs = std::filesystem;

            if (mIsInitialized) {
                return true;
            }

            try {
                // check log path and try to create log directory
                fs::path logPath(logFilePath);
                fs::path logDir = logPath.parent_path();

                if (!fs::exists(logPath)) {
                    fs::create_directories(logDir);
                }

                // initialize spdlog
                constexpr std::size_t logBufferSize = 32 * 1024; // 32Kb
                constexpr std::size_t maxFileSize = 50 * 1024 * 1024; // 50M
                constexpr std::size_t maxNumberOfFiles = 100
                spdlog::init_thread_pool(logBufferSize, 1);
                std::vector<spdlog::sink_ptr> sinks;
                // auto dailySink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(logPath.string(), 0, 2);
                // sinks.push_back(dailySink);

                auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(logPath.string(), maxFileSize, maxNumberOfFiles);
                sinks.push_back(fileSink);

        #if defined(_DEBUG) && defined(_WIN32) && !defined(NO_CONSOLE_LOG)
                auto msSink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
                sinks.push_back(msSink);
        #endif //  _DEBUG

        #if !defined(_WIN32) && !defined(NO_CONSOLE_LOG)
                auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
                sinks.push_back(consoleSink);
        #endif
                spdlog::set_default_logger(std::make_shared<spdlog::logger>("", sinks.begin(), sinks.end()));

                spdlog::set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
                spdlog::flush_on(spdlog::level::warn);
                spdlog::set_level(mLogLevel);
            } catch (std::exception_ptr e) {
                assert(false);
                return false;
            }
            mIsInitialized = true;

            return true;
        }

        void shutdown() { spdlog::shutdown(); }

        template <typename... Args>
        void log(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args &... args) {
            spdlog::log(loc, lvl, fmt, args...);
        }

        template <typename... Args>
        void printf(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args &... args) {
            spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
        }

        spdlog::level::level_enum level() { return _log_level; }

        void setLevel(spdlog::level::level_enum lvl) {
            mLogLevel = lvl;
            spdlog::set_level(lvl);

        }

        void setFlushOn(spdlog::level::level_enum lvl) { spdlog::flush_on(lvl); }

        static const char* getShortname(std::string_view path) {
            if (path.empty()) {
                return path.data();
            }

            size_t pos = path.find_last_of("/\\");
            return path.data() + ((pos == path.npos) ? 0 : pos + 1);
        }

    private:
        std::atomic_bool            mIsInitialized= false;
        spdlog::level::level_enum   mLogLevel = spdlog::level::trace;
    };


    class LoggerNone final : public Singleton<LoggerNone> {
        friend class Singleton<LoggerNone>;
    private:
        LoggerNone(token) = {}

        ~LoggerNone() = default;

        LoggerNone& operator<<(const char* content) { return *this; }
    };

} // namespace eurora::common

#endif // EURORA_COMMON_LOGGER_WLOG_
