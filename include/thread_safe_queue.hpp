#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <mutex>
#include <queue>
#include <condition_variable>
#include <optional>

/**
 * @brief A thread-safe queue implementation wrapper over std::queue.
 * @tparam T The type of elements stored in the queue.
 */
template<typename T>
class ThreadSafeQueue {
private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
    std::condition_variable cond_;

public:
    ThreadSafeQueue() = default;

    // Disable copying to enforce strict thread-safety boundaries
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // Enable moving
    ThreadSafeQueue(ThreadSafeQueue&&) noexcept = default;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) noexcept = default;

    /**
     * @brief Pushes an item into the queue.
     */
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(std::move(value));
        }
        cond_.notify_one();
    }

    /**
     * @brief Attempts to pop an element immediately without blocking.
     * @return std::optional containing the element, or std::nullopt if empty.
     */
    [[nodiscard]] std::optional<T> try_pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) {
            return std::nullopt;
        }
        T value = std::move(queue_.front());
        queue_.pop();
        return value;
    }

    /**
     * @brief Blocks the calling thread until an item becomes available, then pops it.
     */
    void wait_and_pop(T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }
};

#endif // THREAD_SAFE_QUEUE_HPP
