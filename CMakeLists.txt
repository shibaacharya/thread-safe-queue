cmake_minimum_required(VERSION 3.20)
project(ThreadSafeQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# Header-only library target
add_library(thread_safe_queue INTERFACE)
target_include_directories(thread_safe_queue INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(thread_safe_queue INTERFACE Threads::Threads)

# Demo executable
add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE thread_safe_queue)

# Unit tests (Catch2, fetched automatically). Disable with -DBUILD_TESTING=OFF.
option(BUILD_TESTING "Build unit tests" ON)
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
