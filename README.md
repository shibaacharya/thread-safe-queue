# Thread-Safe Queue (Modern C++20)

A lightweight, production-ready, header-only implementation of a **thread-safe queue** using modern C++20 design patterns.

![CI](https://github.com/ShibaAcharya/thread-safe-queue/actions/workflows/ci.yml/badge.svg)

## Features

- **Zero Copy Overhead**: Extensively leverages move semantics (`std::move`).
- **RAII & Safety**: Explicitly deletes copy operations to avoid un-synchronized copies.
- **Modern Return Types**: Uses `std::optional` instead of raw pointers or out-parameters for non-blocking pops.
- **Thread Synchronization**: Uses `std::mutex` and `std::condition_variable` for optimal waiting.

## How to Build & Run

```bash
cmake -B build
cmake --build build
./build/demo
```

## Project Structure

```
thread-safe-queue/
├── include/
│   └── thread_safe_queue.hpp   # Header-only library
├── src/
│   └── main.cpp                # Producer/consumer demo
├── .github/workflows/ci.yml    # CI: builds on every push
├── CMakeLists.txt
└── README.md
```

## API

| Method | Description |
|---|---|
| `void push(T value)` | Pushes an item and notifies one waiting consumer. |
| `std::optional<T> try_pop()` | Non-blocking pop; returns `std::nullopt` if empty. |
| `void wait_and_pop(T& value)` | Blocks until an item is available, then pops it. |

## License

MIT
