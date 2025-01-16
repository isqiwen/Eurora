#include "gtest/gtest.h"

#include "utils/logger/logger.h"

TEST(Test_Log, Test_Log_Initialize) {
    eurora::utils::Logger::Instance().InitFromConfig("config/log_config.json");

    STREAM_INFO() << "Hello World!";
}
