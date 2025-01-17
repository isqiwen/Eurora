#pragma once

#include <memory>
#include <sstream>
#include <string>

#include <fmt/Printf.h>

#include "eurora/utils/export_macros.h"
#include "utils/pattern/singleton.h"

namespace eurora::utils {

enum class EURORA_API LogLevel { Trace, Debug, Info, Warn, Error, Fatal };

class LoggerImpl;

class EURORA_API Logger final : public Singleton<Logger> {
    friend class Singleton<Logger>;

private:
    Logger(Token);

    ~Logger();

public:
    bool InitFromConfig(const std::string& config_file);

    void Shutdown();

    template <typename... Args>
    void Logf(const char* file, int line, const char* function, LogLevel level, const char* fmt, Args&&... args) {
        std::string formatted_message = fmt::format(fmt, std::forward<Args>(args)...);
        Log(file, line, function, level, formatted_message);
    }

    void Log(const char* file, int line, const char* function, LogLevel level, const std::string& message);

    void SetLevel(LogLevel level);

    LogLevel GetLevel() const;

    void FlushOn(LogLevel lvl);

    static const char* GetShortname(std::string_view filepath);

private:
    std::unique_ptr<LoggerImpl> impl_;

public:
    /// let logger like stream
    struct LogStream {
    public:
        LogStream(const char* file, int line, const char* function, LogLevel level) : file_(file), line_(line), function_(function), level_(level) {}

        ~LogStream() { Flush(); }

        template <typename T>
        LogStream& operator<<(const T& value) {
            stream_ << value;
            return *this;
        }

    private:
        void Flush() { Logger::Instance().Log(file_, line_, function_, level_, stream_.str().c_str()); }

    private:
        const char* file_;
        int line_;
        const char* function_;
        LogLevel level_;
        std::ostringstream stream_;
    };
};

#define __FILENAME__ (eurora::utils::Logger::GetShortname(__FILE__))

#define LOG(level, fmt, ...) eurora::utils::Logger::Instance().Log(__FILENAME__, __LINE__, __FUNCTION__, level, fmt, ##__VA_ARGS__)

#define LOG_TRACE(fmt, ...) LOG(LogLevel::Trace, fmt, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LogLevel::Debug, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(LogLevel::Info, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) LOG(LogLevel::Warn, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LogLevel::Error, fmt, ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...) LOG(LogLevel::Fatal, fmt, ##__VA_ARGS__)

#define LOG_IF(condition, level, fmt, ...) \
    if (condition) {                       \
        LOG(level, fmt, ##__VA_ARGS__);    \
    }

#define STREAM_TRACE() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Trace)
#define STREAM_DEBUG() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Debug)
#define STREAM_INFO() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Info)
#define STREAM_WARN() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Warn)
#define STREAM_ERROR() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Error)
#define STREAM_FATAL() eurora::utils::Logger::LogStream(__FILENAME__, __LINE__, __FUNCTION__, LogLevel::Fatal)
}  // namespace eurora::utils
