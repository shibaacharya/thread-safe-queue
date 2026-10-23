cmake_minimum_required(VERSION 3.20)
project(ThreadSafeQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header-only library target
add_library(thread_safe_queue INTERFACE)
target_include_directories(thread_safe_queue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Demo executable
add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE thread_safe_queue)
