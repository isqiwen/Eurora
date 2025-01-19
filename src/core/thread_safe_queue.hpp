#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

#include "eurora/utils/exception.hpp"

EURORA_DEFINE_EXCEPTION(QueueClosed)

namespace eurora::core {
template <typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue()  = default;
    ~ThreadSafeQueue() = default;

    ThreadSafeQueue(const ThreadSafeQueue&)            = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    ThreadSafeQueue(ThreadSafeQueue&&)            = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

    void Push(T);

    T Pop();

    std::optional<T> TryPop();

    void Close();

    bool Empty() const;

    size_t Size() const;

private:
    T PopImpl(std::unique_lock<std::mutex> lock);

private:
    mutable std::mutex mutex_;
    std::condition_variable condition_;
    std::queue<T> queue_;
    bool closed_ = false;
};

template <class T>
T ThreadSafeQueue<T>::PopImpl(std::unique_lock<std::mutex> lock) {
    condition_.wait(lock, [this]() { return !this->queue_.empty() || closed_; });
    if (queue_.empty()) {
        throw QueueClosed();
    }
    T message = std::move(queue_.front());
    queue_.pop();
    return message;
}

template <class T>
void ThreadSafeQueue<T>::Push(T message) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (closed_)
            throw QueueClosed();
        queue_.push(std::move(message));
    }
    condition_.notify_one();
}

template <class T>
T ThreadSafeQueue<T>::Pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    return PopImpl(std::move(lock));
}

template <class T>
std::optional<T> ThreadSafeQueue<T>::TryPop() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        return std::nullopt;
    }
    return PopImpl(std::move(lock));
}

template <class T>
void ThreadSafeQueue<T>::Close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }
    condition_.notify_all();
}

template <class T>
bool ThreadSafeQueue<T>::Empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

template <class T>
size_t ThreadSafeQueue<T>::Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

}  // namespace eurora::core
