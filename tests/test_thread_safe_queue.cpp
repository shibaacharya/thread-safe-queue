#include <catch2/catch_test_macros.hpp>

#include "thread_safe_queue.hpp"

#include <atomic>
#include <chrono>
#include <string>
#include <thread>
#include <type_traits>
#include <vector>

TEST_CASE("push and try_pop returns values in FIFO order", "[thread_safe_queue]") {
    ThreadSafeQueue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);

    REQUIRE(queue.try_pop() == std::optional<int>(1));
    REQUIRE(queue.try_pop() == std::optional<int>(2));
    REQUIRE(queue.try_pop() == std::optional<int>(3));
}

TEST_CASE("try_pop on an empty queue returns std::nullopt", "[thread_safe_queue]") {
    ThreadSafeQueue<std::string> queue;
    REQUIRE(queue.try_pop() == std::nullopt);
}

TEST_CASE("wait_and_pop blocks until an item is pushed", "[thread_safe_queue]") {
    ThreadSafeQueue<int> queue;
    std::atomic<bool> popped{false};
    int value = 0;

    // Catch2 assertions are not thread-safe, so the result is checked on the main thread.
    std::thread consumer([&queue, &popped, &value]() {
        queue.wait_and_pop(value);
        popped = true;
    });

    // Give the consumer a moment to start waiting before we push.
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    REQUIRE_FALSE(popped.load());

    queue.push(42);
    consumer.join();

    REQUIRE(popped.load());
    REQUIRE(value == 42);
}

TEST_CASE("queue handles concurrent producers and consumers without losing items", "[thread_safe_queue]") {
    constexpr int producer_count = 4;
    constexpr int items_per_producer = 250;
    constexpr int total_items = producer_count * items_per_producer;

    ThreadSafeQueue<int> queue;
    std::vector<std::thread> producers;
    producers.reserve(producer_count);

    for (int p = 0; p < producer_count; ++p) {
        producers.emplace_back([&queue, items_per_producer]() {
            for (int i = 0; i < items_per_producer; ++i) {
                queue.push(i);
            }
        });
    }

    std::atomic<int> consumed{0};
    std::thread consumer([&queue, &consumed, total_items]() {
        int value = 0;
        while (consumed.load() < total_items) {
            queue.wait_and_pop(value);
            ++consumed;
        }
    });

    for (auto& t : producers) {
        t.join();
    }
    consumer.join();

    REQUIRE(consumed.load() == total_items);
    REQUIRE(queue.try_pop() == std::nullopt);
}

TEST_CASE("ThreadSafeQueue is movable but not copyable", "[thread_safe_queue]") {
    STATIC_REQUIRE_FALSE(std::is_copy_constructible_v<ThreadSafeQueue<int>>);
    STATIC_REQUIRE_FALSE(std::is_copy_assignable_v<ThreadSafeQueue<int>>);
    STATIC_REQUIRE(std::is_move_constructible_v<ThreadSafeQueue<int>>);
    STATIC_REQUIRE(std::is_move_assignable_v<ThreadSafeQueue<int>>);
}
