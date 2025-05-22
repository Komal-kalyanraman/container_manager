/**
 * @file posix_message_queue_consumer.cpp
 * @brief Implements the MessageQueueConsumer class for handling messages from a POSIX message queue.
 */

#include "inc/posix_message_queue_consumer.hpp"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include <mqueue.h>
#include "inc/common.hpp"

/**
 * @brief Constructs a MessageQueueConsumer with the given configuration and executor.
 * @param config Configuration for the message queue consumer.
 * @param executor Shared pointer to a RequestExecutor for processing messages.
 */
MessageQueueConsumer::MessageQueueConsumer(const MessageQueueConfig& config, std::shared_ptr<RequestExecutor> executor)
    : config_(config), executor_(std::move(executor)), running_(false) {}

/**
 * @brief Destructor. Stops the consumer if running.
 */
MessageQueueConsumer::~MessageQueueConsumer() {
    Stop();
}

/**
 * @brief Starts the message queue consumer loop in a separate thread.
 */
void MessageQueueConsumer::Start() {
    running_ = true;
    consumer_thread_ = std::thread(&MessageQueueConsumer::ConsumeLoop, this);
}

/**
 * @brief Stops the message queue consumer loop and joins the thread.
 */
void MessageQueueConsumer::Stop() {
    running_ = false;
    if (consumer_thread_.joinable()) {
        consumer_thread_.join();
    }
}

/**
 * @brief Main loop for consuming messages from the POSIX message queue.
 *        Receives messages and dispatches them to the executor.
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
        std::cerr << "[MQ] Failed to open message queue: " << strerror(errno) << std::endl;
        return;
    }

    char* buffer = new char[config_.MaxMsgSize];        // Buffer for incoming messages

    // Main loop: receive and process messages while running_
    while (running_) {
        ssize_t bytes_read = mq_receive(mqd, buffer, config_.MaxMsgSize, nullptr);
        if (bytes_read > 0) {
            std::string payload(buffer, bytes_read);
            std::cout << "[MQ] Received message: " << payload << std::endl;
            try {
                executor_->Execute(payload);            // Process the message
            } catch (const std::exception& e) {
                std::cerr << "[MQ] Error executing request: " << e.what() << std::endl;
            }
        } else if (bytes_read == -1 && errno == EMSGSIZE) {
            std::cerr << "[MQ] mq_receive error: Message too long for buffer (" << config_.MaxMsgSize << " bytes)" << std::endl;
            break;
        }
        usleep(config_.SleepTime);
    }
    delete[] buffer;
    mq_close(mqd); // Clean up
}