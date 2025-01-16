#pragma once

#include <atomic>
#include <condition_variable>
#include <list>
#include <mutex>

namespace eurora::core {

template <class T>
class MPMCChannel {
public:
    MPMCChannel() : size_(0) {}

    MPMCChannel(MPMCChannel&&) noexcept;

    void push(T);
    template <class... ARGS>
    void emplace(ARGS&&... args);

    T pop();
    optional<T> try_pop();

    void close();
    size_t size() const;  // 新增的接口：获取当前队列大小
    bool empty() const;   // 新增的接口：判断队列是否为空

private:
    T pop_impl(std::unique_lock<std::mutex>& lock);

    std::list<T> queue_;
    std::atomic<size_t> size_;  // 线程安全的大小统计
    bool closed_ = false;
    std::mutex m_;
    std::condition_variable cv_;
};

class ChannelClosed : public std::runtime_error {
public:
    ChannelClosed() : std::runtime_error("Channel was closed") {};
};

/** Implementation **/

template <class T>
T MPMCChannel<T>::pop_impl(std::unique_lock<std::mutex>& lock) {
    cv_.wait(lock, [this]() { return !queue_.empty() || closed_; });

    if (queue_.empty()) {
        throw ChannelClosed();
    }

    T value = std::move(queue_.front());
    queue_.pop_front();
    --size_;  // 更新大小
    return value;
}

template <class T>
T MPMCChannel<T>::pop() {
    std::unique_lock<std::mutex> lock(m_);
    return pop_impl(lock);
}

template <class T>
optional<T> MPMCChannel<T>::try_pop() {
    std::unique_lock<std::mutex> lock(m_);
    if (queue_.empty()) {
        return none;
    }
    return pop_impl(lock);
}

template <class T>
void MPMCChannel<T>::push(T message) {
    {
        std::lock_guard<std::mutex> lock(m_);
        if (closed_)
            throw ChannelClosed();
        queue_.emplace_back(std::move(message));
        ++size_;  // 更新大小
    }
    cv_.notify_one();
}

template <class T>
template <class... ARGS>
void MPMCChannel<T>::emplace(ARGS&&... args) {
    {
        std::lock_guard<std::mutex> guard(m_);
        if (closed_)
            throw ChannelClosed();
        queue_.emplace_back(std::forward<ARGS>(args)...);
        ++size_;  // 更新大小
    }
    cv_.notify_one();
}

template <class T>
void MPMCChannel<T>::close() {
    {
        std::lock_guard<std::mutex> lock(m_);
        closed_ = true;
    }
    cv_.notify_all();
}

template <class T>
size_t MPMCChannel<T>::size() const {
    return size_.load();  // 线程安全地返回大小
}

template <class T>
bool MPMCChannel<T>::empty() const {
    return size_.load() == 0;  // 线程安全地检查是否为空
}

template <class T>
MPMCChannel<T>::MPMCChannel(MPMCChannel&& other) noexcept {
    std::lock_guard<std::mutex> guard(other.m_);
    this->queue_  = std::move(other.queue_);
    this->size_   = other.size_.load();
    this->closed_ = other.closed_;
    other.closed_ = true;
}

}  // namespace eurora::core
