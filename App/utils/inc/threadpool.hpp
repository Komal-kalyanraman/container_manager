/**
 * @file threadpool.hpp
 * @brief Declares the ThreadPool utility class for managing worker threads.
 */

#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

/// @class ThreadPool
/// @brief Provides a fixed-size pool of threads for executing background tasks.
/// @details This class allows tasks to be enqueued and executed concurrently by a pool of worker threads.
class ThreadPool {
public:
    /**
     * @brief Constructs a ThreadPool with the specified number of worker threads.
     * @param num_threads The number of threads in the pool.
     */
    explicit ThreadPool(size_t num_threads);

    /**
     * @brief Destructor. Joins all threads and cleans up resources.
     */
    ~ThreadPool();

    /**
     * @brief Enqueues a task to be executed by the thread pool.
     * @param task The function to execute.
     */
    void Enqueue(std::function<void()> task);

private:
    std::vector<std::thread> workers_;           ///< Worker threads.
    std::queue<std::function<void()>> tasks_;    ///< Task queue.
    std::mutex queue_mutex_;                     ///< Mutex for synchronizing access to the queue.
    std::condition_variable condition_;          ///< Condition variable for task notification.
    std::atomic<bool> stop_{false};              ///< Flag to signal stopping of the pool.

    /**
     * @brief Worker function executed by each thread.
     */
    void Worker();
};