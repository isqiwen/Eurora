#include <gtest/gtest.h>
#include <chrono>
#include <thread>

#include "utils/time/time_utils.h"

using namespace eurora::utils;

class TimeUtilsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Any setup code if needed
    }

    void TearDown() override {
        // Any cleanup code if needed
    }
};

// Test: GetCurrentDateTimeStringInMS
TEST_F(TimeUtilsTest, GetCurrentDateTimeStringInMS) {
    std::string datetime = TimeUtils::GetCurrentDateTimeStringInMS();
    ASSERT_FALSE(datetime.empty());
    std::cout << "Current DateTime in MS: " << datetime << std::endl;
}

// Test: GetTimestampString
TEST_F(TimeUtilsTest, GetTimestampString) {
    std::string timestamp = TimeUtils::GetTimestampString();
    ASSERT_FALSE(timestamp.empty());
    std::cout << "Timestamp: " << timestamp << std::endl;
}

// Test: GetTimestampWithMsString
TEST_F(TimeUtilsTest, GetTimestampWithMsString) {
    std::string timestamp_with_ms = TimeUtils::GetTimestampWithMsString();
    ASSERT_FALSE(timestamp_with_ms.empty());
    std::cout << "Timestamp with MS: " << timestamp_with_ms << std::endl;
}

// Test: GetCurrentDateString
TEST_F(TimeUtilsTest, GetCurrentDateString) {
    std::string current_date = TimeUtils::GetCurrentDateString();
    ASSERT_FALSE(current_date.empty());
    std::cout << "Current Date: " << current_date << std::endl;

    // Validate format (e.g., "YYYY-MM-DD")
    ASSERT_EQ(current_date.size(), 10);
    ASSERT_EQ(current_date[4], '-');
    ASSERT_EQ(current_date[7], '-');
}

// Test: GetCurrentTimeString
TEST_F(TimeUtilsTest, GetCurrentTimeString) {
    std::string current_time = TimeUtils::GetCurrentTimeString();
    ASSERT_FALSE(current_time.empty());
    std::cout << "Current Time: " << current_time << std::endl;

    // Validate format (e.g., "HH:MM:SS")
    ASSERT_EQ(current_time.size(), 8);
    ASSERT_EQ(current_time[2], ':');
    ASSERT_EQ(current_time[5], ':');
}

// Test: GetCurrentTimeString with milliseconds
TEST_F(TimeUtilsTest, GetCurrentTimeStringWithMs) {
    std::string current_time_with_ms = TimeUtils::GetCurrentTimeString(true);
    ASSERT_FALSE(current_time_with_ms.empty());
    std::cout << "Current Time with MS: " << current_time_with_ms << std::endl;

    // Validate format (e.g., "HH:MM:SS.MS")
    ASSERT_GE(current_time_with_ms.size(), 12);
    ASSERT_EQ(current_time_with_ms[2], ':');
    ASSERT_EQ(current_time_with_ms[5], ':');
    ASSERT_EQ(current_time_with_ms[8], '.');
}
