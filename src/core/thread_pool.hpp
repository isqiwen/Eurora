#include <atomic>
#include <functional>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

#include "thread_safe_queue.hpp"

namespace eurora::core {
class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads) : stop_(false) {
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this]() {
                try {
                    while (true) {
                        tasks_.Pop()();
                    }
                } catch (const QueueClosed&) {}
            });
        }
    }

    ~ThreadPool() { Stop(); }

    ThreadPool(const ThreadPool&)            = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    ThreadPool(ThreadPool&&)            = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template <typename F, typename... Args>
    auto Push(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using ReturnType = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        auto result = task->get_future();
        {
            if (stop_.load()) {
                throw std::runtime_error("Cannot push task on a stopped ThreadPool");
            }
            tasks_.Push([task]() { (*task)(); });
        }
        return result;
    }

    void Stop() {
        if (!stop_.exchange(true)) {
            tasks_.Close();

            for (std::thread& worker : workers_) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
        }
    }

private:
    std::vector<std::thread> workers_;
    ThreadSafeQueue<std::function<void()>> tasks_;
    std::atomic<bool> stop_;
};

}  // namespace eurora::core
