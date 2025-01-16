#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

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

class QueueClosed : public std::runtime_error {
public:
    QueueClosed() : std::runtime_error("Queue was closed") {};
};

template <class T>
T ThreadSafeQueue<T>::PopImpl(std::unique_lock<std::mutex> lock) {
    condition_.wait(lock, [this]() { return !this->queue_.empty() || closed_; });
    if (queue_.empty()) {
        throw QueueClosed();
    }
    T message = std::move(queue.front());
    queue_.pop_front();
    return message;
}

template <class T>
void ThreadSafeQueue<T>::Push(T message) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (closed_)
            throw QueueClosed();
        queue_.emplace_back(std::move(message));
    }
    cv.notify_one();
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
        std::nullopt;
    }
    return PopImpl(std::move(lock));
}

void ThreadSafeQueue::Close() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
    }
    condition_.notify_all();
}

bool ThreadSafeQueue::Empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

size_t ThreadSafeQueue::Size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

}  // namespace eurora::core
