/**
 * @file message_queue_consumer.hpp
 * @brief Declares the MessageQueueConsumer class for handling messages from a POSIX message queue.
 */

#pragma once

#include <memory>
#include <string>
#include <thread>

#include "inc/common.hpp"
#include "inc/request_executor.hpp"

/// @class MessageQueueConsumer
/// @brief Consumes messages from a POSIX message queue and dispatches them to a RequestExecutor.
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
    void ConsumeLoop();

    MessageQueueConfig config_;
    std::shared_ptr<RequestExecutor> executor_;
    bool running_;
    std::thread consumer_thread_;
};