#include <gtest/gtest.h>
#include <fstream>

#include "eurora/utils/logger.h"

using namespace eurora::utils;

// Helper function to create a temporary configuration file
std::string CreateTempConfig() {
    std::string temp_config = "temp_log_config.json";
    std::ofstream file(temp_config);
    file << R"({
        "log_level": "debug",
        "log_flush_level": "warn",
        "enable_console_log": true,
        "enable_file_log": true,
        "log_file_path": "logs/test_log.log",
        "log_pattern": "%s(%#): [%l %D %T.%e %t %!] %v",
        "max_file_size": 1048576,
        "max_files": 5,
        "log_buffer_size": 32768,
        "log_thread_pool_size": 1
    })";
    file.close();
    return temp_config;
}

// Test suite for Logger
class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        temp_config_ = CreateTempConfig();
        Logger::Instance().InitFromConfig(temp_config_);
    }

    void TearDown() override {
        Logger::Instance().Shutdown();
        remove(temp_config_.c_str());
    }

    std::string temp_config_;
};

TEST_F(LoggerTest, InitializationFromConfig) { EXPECT_NO_THROW(Logger::Instance().InitFromConfig(temp_config_)); }

TEST_F(LoggerTest, LogSimpleMessage) {
    EXPECT_NO_THROW(LOG_INFO("This is an info log message."));
    EXPECT_NO_THROW(LOG_DEBUG("This is a debug log message."));
    EXPECT_NO_THROW(LOG_ERROR("This is an error log message."));
}

TEST_F(LoggerTest, LogWithFormat) {
    EXPECT_NO_THROW(LOG_INFO("Formatted number: {}", 42));
    EXPECT_NO_THROW(LOG_WARN("Multiple values: {}, {}", "Value1", 123));
}

TEST_F(LoggerTest, StreamLogging) {
    EXPECT_NO_THROW(STREAM_INFO() << "Streamed log message with value: " << 3.14);
    EXPECT_NO_THROW(STREAM_WARN() << "Another streamed log, value: " << 123 << " " << 0.75);
}

TEST_F(LoggerTest, SetAndGetLogLevel) {
    Logger::Instance().SetLevel(LogLevel::Info);
    EXPECT_EQ(Logger::Instance().GetLevel(), LogLevel::Info);

    Logger::Instance().SetLevel(LogLevel::Debug);
    EXPECT_EQ(Logger::Instance().GetLevel(), LogLevel::Debug);
}

TEST_F(LoggerTest, FlushOnSpecificLevel) { EXPECT_NO_THROW(Logger::Instance().FlushOn(LogLevel::Warn)); }

TEST_F(LoggerTest, ShutdownLogger) { EXPECT_NO_THROW(Logger::Instance().Shutdown()); }
