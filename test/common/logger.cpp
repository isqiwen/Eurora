#include "gtest/gtest.h"

#include "common/logger/logger.h"

TEST(Test_Log, Test_Log_Initialize) {
  eurora::common::Logger::Instance().Init("logs/test_log_initialize.log");
  eurora::common::Logger::Instance().set_level(LOG_LEVEL_INFO);

  STREAM_INFO() << "Hello World!";
}
