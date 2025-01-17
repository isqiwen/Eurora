#pragma once

#include <spdlog/spdlog.h>

#include "logger_impl.h"

namespace eurora::utils {

class LoggerImplSpdlog : public LoggerImpl {
public:
    LoggerImplSpdlog() : logger_(nullptr) {}

    virtual ~LoggerImpl() override { Shutdown(); }

    virtual bool InitFromConfig(const std::string& config_file) override;

    virtual void Log(const char* file, int line, const char* function, LogLevel level, const std::string& message) override;

    virtual void Shutdown() override;

    virtual void SetLevel(LogLevel level) override;

    virtual LogLevel GetLevel() const override;

    virtual void FlushOn(LogLevel lvl) override;

private:
    spdlog::level::level_enum ToSpdlogLevel(LogLevel level) const {
        switch (level) {
            case LogLevel::Trace:
                return spdlog::level::trace;
            case LogLevel::Debug:
                return spdlog::level::debug;
            case LogLevel::Info:
                return spdlog::level::info;
            case LogLevel::Warn:
                return spdlog::level::warn;
            case LogLevel::Error:
                return spdlog::level::err;
            case LogLevel::Fatal:
                return spdlog::level::critical;
            default:
                return spdlog::level::info;
        }
    }

    LogLevel FromSpdlogLevel(spdlog::level::level_enum level) const {
        switch (level) {
            case spdlog::level::trace:
                return LogLevel::Trace;
            case spdlog::level::debug:
                return LogLevel::Debug;
            case spdlog::level::info:
                return LogLevel::Info;
            case spdlog::level::warn:
                return LogLevel::Warn;
            case spdlog::level::err:
                return LogLevel::Error;
            case spdlog::level::critical:
                return LogLevel::Fatal;
            default:
                return LogLevel::Info;
        }
    }

private:
    bool initialized_ = false;
    std::shared_ptr<spdlog::logger> logger_;
};

}  // namespace eurora::utils
