/**
 * @file http_server.hpp
 * @brief Defines the HttpServerHandler class for handling HTTP requests.
 */

#pragma once
#include <memory>
#include <string>

#include "inc/common.hpp"
#include "inc/threadpool.hpp"
#include "inc/request_executor.hpp"

/**
 * @class HttpServerHandler
 * @brief Handles incoming HTTP requests and dispatches them to the appropriate JSON/Protobuf executor.
 * @details This class manages the HTTP server lifecycle and uses a thread pool to process requests concurrently.
 */
class HttpServerHandler {
public:
    /**
     * @brief Constructs an HTTP server handler.
     * @param executor Shared pointer to a RequestExecutor for processing requests.
     * @param thread_pool_size Number of threads in the thread pool for handling requests.
     */
    explicit HttpServerHandler(std::shared_ptr<RequestExecutor> executor, int thread_pool_size);

    /**
     * @brief Starts the HTTP server on the specified host and port.
     * @param host The host address of the HTTP server.
     * @param port The port on which the server will listen for incoming requests.
     */
    void Start(const std::string& host, int port);

private:
    /**
     * @brief Shared pointer to the request executor used for processing HTTP requests.
     */
    std::shared_ptr<RequestExecutor> executor_;

    /**
     * @brief Unique pointer to the thread pool for handling requests concurrently.
     */
    std::unique_ptr<ThreadPool> pool_;
};