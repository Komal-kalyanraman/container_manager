/**
 * @file threadpool.cpp
 * @brief Implements the ThreadPool utility class for managing worker threads.
 */

#include "inc/threadpool.hpp"

/**
 * @brief Constructs a ThreadPool with the specified number of worker threads.
 * @param num_threads The number of threads in the pool.
 */
ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back([this] { Worker(); });
    }
}

/**
 * @brief Destructor. Signals all threads to stop and joins them.
 */
ThreadPool::~ThreadPool() {
    stop_ = true;
    condition_.notify_all();
    for (auto& worker : workers_) {
        if (worker.joinable()) worker.join();
    }
}

/**
 * @brief Enqueues a task to be executed by the thread pool.
 * @param task The function to execute.
 */
void ThreadPool::Enqueue(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        tasks_.push(std::move(task));
    }
    condition_.notify_one();
}

/**
 * @brief Worker function executed by each thread. Waits for and executes tasks from the queue.
 */
void ThreadPool::Worker() {
    while (!stop_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
            if (stop_ && tasks_.empty()) return;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        task();
    }
}