/**
 * @file posix_message_queue_consumer.hpp
 * @brief Declares the MessageQueueConsumer class for handling messages from a POSIX message queue.
 */

#pragma once

#include <memory>
#include <string>
#include <thread>

#include "inc/common.hpp"
#include "inc/request_executor.hpp"

/**
 * @class MessageQueueConsumer
 * @brief Consumes messages from a POSIX message queue and dispatches them to a RequestExecutor.
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
     * @brief Destructor.
     */
    ~MessageQueueConsumer();

    /**
     * @brief Starts the message queue consumer loop in a separate thread.
     */
    void Start();

    /**
     * @brief Stops the message queue consumer loop.
     */
    void Stop();

private:
    /**
     * @brief Main loop for consuming messages from the POSIX message queue.
     */
    void ConsumeLoop();

    MessageQueueConfig config_;                      ///< Configuration for the message queue consumer.
    std::shared_ptr<RequestExecutor> executor_;      ///< Shared pointer to the request executor.
    bool running_;                                   ///< Indicates if the consumer is running.
    std::thread consumer_thread_;                    ///< Thread for running the consumer loop.
};