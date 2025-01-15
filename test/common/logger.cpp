#include "gtest/gtest.h"

#include "common/logger/logger.h"

TEST(Test_Log, Test_Log_Initialize) {
  eurora::common::Logger::Instance().InitFromConfig("config/log_config.json");

  STREAM_INFO() << "Hello World!";
}
