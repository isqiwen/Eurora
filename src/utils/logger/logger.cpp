#include "eurora/utils/logger.h"
#include "logger_impl.h"

namespace eurora::utils {

Logger::Logger(Token) : impl_(std::make_unique<LoggerImpl>()) {}

Logger::~Logger() { Shutdown(); }

bool Logger::InitFromConfig(const std::string& config_file) { return impl_->InitFromConfig(config_file); }

void Logger::Log(const char* file, int line, const char* function, LogLevel level, const std::string& message) {
    impl_->Log(file, line, function, level, message);
}

void Logger::Shutdown() { impl_->Shutdown(); }

void Logger::FlushOn(LogLevel lvl) { impl_->FlushOn(lvl); }

void Logger::SetLevel(LogLevel level) { impl_->SetLevel(level); }

LogLevel Logger::GetLevel() const { return impl_->GetLevel(); }

const char* Logger::GetShortname(std::string_view filepath) {
    if (filepath.empty()) {
        return filepath.data();
    }

    return std::filesystem::path(filepath).filename().string().c_str();
}

}  // namespace eurora::utils
