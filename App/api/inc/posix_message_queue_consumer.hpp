/**
 * @file posix_message_queue_consumer.hpp
 * @brief Declares the MessageQueueConsumer class for handling messages from a POSIX message queue.
 */

#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include "inc/common.hpp"
#include "inc/request_executor.hpp"

/**
 * @class MessageQueueConsumer
 * @brief Consumes messages from a POSIX message queue and dispatches them to a RequestExecutor.
 *
 * This class manages the lifecycle of a background thread that listens for messages
 * on a POSIX message queue. Received messages are passed to the provided RequestExecutor
 * for processing. The consumer supports graceful shutdown via a thread-safe flag and
 * uses a configurable timeout for message queue operations.
 */
class MessageQueueConsumer {
public:
    /**
     * @brief Constructs a MessageQueueConsumer.
     * @param config Configuration for the message queue consumer.
     * @param executor Shared pointer to a RequestExecutor for processing messages.
     */
    MessageQueueConsumer(const MessageQueueConfig& config, std::shared_ptr<RequestExecutor> executor);

    /**
     * @brief Destructor. Ensures the consumer is stopped and resources are cleaned up.
     */
    ~MessageQueueConsumer();

    /**
     * @brief Starts the message queue consumer loop in a separate thread.
     *
     * Sets the running flag to true and launches the consumer loop in a new thread.
     */
    void Start();

    /**
     * @brief Stops the message queue consumer loop.
     *
     * Sets the running flag to false, signaling the consumer loop to exit.
     * Waits for the consumer thread to finish execution.
     */
    void Stop();

private:
    /**
     * @brief Main loop for consuming messages from the POSIX message queue.
     *
     * Opens (or creates) the message queue with the specified attributes.
     * Repeatedly waits for and receives messages using mq_timedreceive with a timeout,
     * allowing for graceful shutdown when Stop() is called.
     * Each received message is dispatched to the executor for processing.
     * Handles errors and cleans up resources on exit.
     */
    void ConsumeLoop();

    MessageQueueConfig config_;                      ///< Configuration for the message queue consumer.
    std::shared_ptr<RequestExecutor> executor_;      ///< Shared pointer to the request executor.
    std::thread consumer_thread_;                    ///< Thread for running the consumer loop.
    std::atomic<bool> running_{false};              ///< Flag indicating if the consumer loop should keep running.
};