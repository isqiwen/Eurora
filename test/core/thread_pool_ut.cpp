#include <gtest/gtest.h>
#include <atomic>
#include <thread>
#include <vector>
#include "core/thread_pool.hpp"

using namespace eurora::utils;
using namespace eurora::core;

class ThreadPoolTest : public ::testing::Test {};

TEST_F(ThreadPoolTest, BasicTaskExecution) {
    ThreadPool pool(4);

    std::atomic<int> counter{0};

    for (int i = 0; i < 10; ++i) {
        pool.Push([&counter]() { ++counter; });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_EQ(counter.load(), 10);
}

TEST_F(ThreadPoolTest, TaskWithReturnValue) {
    ThreadPool pool(2);

    auto future = pool.Push([](int a, int b) { return a + b; }, 3, 7);

    EXPECT_EQ(future.get(), 10);
}

TEST_F(ThreadPoolTest, ConcurrentExecution) {
    ThreadPool pool(4);

    std::atomic<int> counter{0};

    for (int i = 0; i < 100; ++i) {
        pool.Push([&counter]() { ++counter; });
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    EXPECT_EQ(counter.load(), 100);
}

TEST_F(ThreadPoolTest, StopPool) {
    ThreadPool pool(2);

    std::atomic<int> counter{0};

    for (int i = 0; i < 5; ++i) {
        pool.Push([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            ++counter;
        });
    }

    pool.Stop();

    EXPECT_EQ(counter.load(), 5);
}

TEST_F(ThreadPoolTest, PushAfterStop) {
    ThreadPool pool(2);

    pool.Stop();

    EXPECT_THROW(pool.Push([]() {}), std::runtime_error);
}

TEST_F(ThreadPoolTest, TaskWithException) {
    ThreadPool pool(2);

    auto future = pool.Push([]() { throw std::runtime_error("Task error"); });

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST_F(ThreadPoolTest, TaskOverflow) {
    ThreadPool pool(2);

    std::atomic<int> counter{0};

    for (int i = 0; i < 100; ++i) {
        pool.Push([&counter]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            ++counter;
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_EQ(counter.load(), 100);
}
