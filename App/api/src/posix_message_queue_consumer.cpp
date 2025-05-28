/**
 * @file posix_message_queue_consumer.cpp
 * @brief Implements the MessageQueueConsumer class for handling messages from a POSIX message queue.
 */

#include "inc/posix_message_queue_consumer.hpp"

#include <ctime>
#include <cstring>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <mqueue.h>
#include "inc/common.hpp"
#include "inc/logging.hpp"

/**
 * @brief Constructs a MessageQueueConsumer with the given configuration and executor.
 * @param config Configuration for the message queue consumer.
 * @param executor Shared pointer to a RequestExecutor for processing messages.
 */
MessageQueueConsumer::MessageQueueConsumer(const MessageQueueConfig& config, std::shared_ptr<RequestExecutor> executor)
    : config_(config), executor_(std::move(executor)), running_(false) {}

/**
 * @brief Destructor. Ensures the consumer is stopped and resources are cleaned up.
 */
MessageQueueConsumer::~MessageQueueConsumer() {
    Stop();
}

/**
 * @brief Starts the message queue consumer loop in a separate thread.
 * 
 * Sets the running flag to true and launches the consumer loop in a new thread.
 */
void MessageQueueConsumer::Start() {
    running_ = true;
    consumer_thread_ = std::thread(&MessageQueueConsumer::ConsumeLoop, this);
}

/**
 * @brief Stops the message queue consumer loop and joins the thread.
 * 
 * Sets the running flag to false, signaling the consumer loop to exit.
 * Waits for the consumer thread to finish execution.
 */
void MessageQueueConsumer::Stop() {
    running_ = false;
    if (consumer_thread_.joinable()) {
        consumer_thread_.join();
    }
}

/**
 * @brief Main loop for consuming messages from the POSIX message queue.
 * 
 * Opens (or creates) the message queue with the specified attributes.
 * Repeatedly waits for and receives messages using mq_timedreceive with a timeout,
 * allowing for graceful shutdown when Stop() is called.
 * Each received message is dispatched to the executor for processing.
 * Handles errors and cleans up resources on exit.
 */
void MessageQueueConsumer::ConsumeLoop() {
    // Set message queue attributes to ensure correct message size and capacity
    struct mq_attr attr;
    attr.mq_flags = 0;                                  // Blocking mode
    attr.mq_maxmsg = config_.MaxMsgCount;               // Max number of messages in the queue
    attr.mq_msgsize = config_.MaxMsgSize;               // Max size of each message (must match buffer)
    attr.mq_curmsgs = 0;                                // Ignored for mq_open

    // Open or create the message queue with the specified attributes
    mqd_t mqd = mq_open(config_.QueueName.c_str(), O_RDONLY | O_CREAT, 0644, &attr);
    if (mqd == (mqd_t)-1) {
        CM_LOG_ERROR << "[MQ] Failed to open message queue: " << strerror(errno) << std::endl;
        return;
    }

    // Use a smart pointer for the buffer to ensure automatic cleanup
    std::unique_ptr<char[]> buffer(new char[config_.MaxMsgSize]);

    // Main loop: receive and process messages while running_
    while (running_) {
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += config_.ReceiveTimeoutSec; // Use the configured timeout

        ssize_t bytes_read = mq_timedreceive(mqd, buffer.get(), config_.MaxMsgSize, nullptr, &ts);
        if (bytes_read > 0) {
            // Message received successfully
            std::string payload(buffer.get(), bytes_read);
            CM_LOG_INFO << "[MQ] Received message: " << payload << std::endl;
            try {
                auto result = executor_->Execute(payload);
                // Log the result status
                if (result.contains("status")) {
                    if (result["status"] == "error") {
                        CM_LOG_ERROR << "[MQ] Execution error: "
                                     << result.value("message", "Unknown error") << std::endl;
                    } else {
                        CM_LOG_INFO << "[MQ] Execution success." << std::endl;
                    }
                } else {
                    CM_LOG_WARN << "[MQ] No status field in execution result." << std::endl;
                }
            } catch (const std::exception& e) {
                CM_LOG_ERROR << "[MQ] Error executing request: " << e.what() << std::endl;
            }
        } else if (bytes_read == -1 && errno == ETIMEDOUT) {
            // Timeout: just loop again and check running_
            continue;
        } else if (bytes_read == -1 && errno == EMSGSIZE) {
            CM_LOG_ERROR << "[MQ] mq_receive error: Message too long for buffer (" << config_.MaxMsgSize << " bytes)" << std::endl;
            break;
        } else if (bytes_read == -1) {
            CM_LOG_ERROR << "[MQ] mq_timedreceive error: " << strerror(errno) << std::endl;
            break;
        }
    }
    mq_close(mqd); // Clean up
}