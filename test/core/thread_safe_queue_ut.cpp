#include <gtest/gtest.h>
#include <chrono>
#include <optional>
#include <thread>
#include <vector>
#include "core/thread_safe_queue.hpp"

using namespace eurora::utils;
using namespace eurora::core;

class ThreadSafeQueueTest : public ::testing::Test {};

TEST_F(ThreadSafeQueueTest, PushAndPop) {
    ThreadSafeQueue<int> queue;

    queue.Push(1);
    queue.Push(2);
    queue.Push(3);

    EXPECT_EQ(queue.Pop(), 1);
    EXPECT_EQ(queue.Pop(), 2);
    EXPECT_EQ(queue.Pop(), 3);

    EXPECT_TRUE(queue.Empty());
}

TEST_F(ThreadSafeQueueTest, TryPop) {
    ThreadSafeQueue<int> queue;

    queue.Push(42);

    auto result = queue.TryPop();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 42);

    result = queue.TryPop();
    EXPECT_FALSE(result.has_value());
}

TEST_F(ThreadSafeQueueTest, Close) {
    ThreadSafeQueue<int> queue;

    queue.Close();

    EXPECT_THROW(queue.Pop(), QueueClosed);

    EXPECT_THROW(queue.Push(20), QueueClosed);

    auto result = queue.TryPop();
    EXPECT_FALSE(result.has_value());
}

TEST_F(ThreadSafeQueueTest, ConcurrentPushAndPop) {
    ThreadSafeQueue<int> queue;
    constexpr int kNumElements = 100;

    auto producer = [&]() {
        for (int i = 0; i < kNumElements; ++i) {
            queue.Push(i);
        }
    };

    auto consumer = [&]() {
        for (int i = 0; i < kNumElements; ++i) {
            EXPECT_NO_THROW(queue.Pop());
        }
    };

    std::thread producer_thread(producer);
    std::thread consumer_thread(consumer);

    producer_thread.join();
    consumer_thread.join();

    EXPECT_TRUE(queue.Empty());
}

TEST_F(ThreadSafeQueueTest, Size) {
    ThreadSafeQueue<int> queue;

    EXPECT_EQ(queue.Size(), 0);

    queue.Push(1);
    queue.Push(2);

    EXPECT_EQ(queue.Size(), 2);

    queue.Pop();

    EXPECT_EQ(queue.Size(), 1);
}

TEST_F(ThreadSafeQueueTest, BlockingPop) {
    ThreadSafeQueue<int> queue;

    std::thread producer([&queue]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        queue.Push(99);
    });

    auto result = queue.Pop();
    EXPECT_EQ(result, 99);

    producer.join();
}

TEST_F(ThreadSafeQueueTest, MultipleProducersAndConsumers) {
    ThreadSafeQueue<int> queue;
    constexpr int kNumProducers           = 4;
    constexpr int kNumConsumers           = 4;
    constexpr int kNumElementsPerProducer = 25;

    std::vector<std::thread> producers, consumers;
    std::atomic<int> consumed_count(0);

    for (int i = 0; i < kNumProducers; ++i) {
        producers.emplace_back([&queue, i]() {
            for (int j = 0; j < kNumElementsPerProducer; ++j) {
                queue.Push(i * kNumElementsPerProducer + j);
            }
        });
    }

    for (int i = 0; i < kNumConsumers; ++i) {
        consumers.emplace_back([&queue, &consumed_count]() {
            while (consumed_count < kNumProducers * kNumElementsPerProducer) {
                auto result = queue.TryPop();
                if (result.has_value()) {
                    ++consumed_count;
                }
            }
        });
    }

    for (auto& producer : producers) {
        producer.join();
    }

    queue.Close();

    for (auto& consumer : consumers) {
        consumer.join();
    }

    EXPECT_EQ(consumed_count, kNumProducers * kNumElementsPerProducer);
}
