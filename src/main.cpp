#include "thread_safe_queue.hpp"

#include <iostream>
#include <thread>
#include <chrono>

int main() {
    ThreadSafeQueue<int> queue;

    // Producer thread
    std::jthread producer([&queue]() {
        for (int i = 1; i <= 5; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::cout << "[Producer] Pushing: " << i << "\n";
            queue.push(i);
        }
    });

    // Consumer thread
    std::jthread consumer([&queue]() {
        for (int i = 1; i <= 5; ++i) {
            int value;
            queue.wait_and_pop(value);
            std::cout << "[Consumer] Popped: " << value << "\n";
        }
    });

    return 0;
}
