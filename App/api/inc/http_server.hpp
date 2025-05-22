/**
 * @file http_server.hpp
 * @brief Defines the HttpServerHandler class for handling HTTP requests.
 */

#pragma once
#include <memory>
#include <string>
#include <httplib.h>

#include "inc/common.hpp"
#include "inc/threadpool.hpp"
#include "inc/request_executor.hpp"

/**
 * @class HttpServerHandler
 * @brief Handles incoming HTTP requests and dispatches them to the appropriate JSON/Protobuf executor.
 *
 * This class manages the HTTP server lifecycle using cpp-httplib and processes requests
 * concurrently using a thread pool. It supports starting and stopping the server for
 * graceful shutdown.
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
     *
     * This method blocks until the server is stopped.
     */
    void Start(const std::string& host, int port);

    /**
     * @brief Stops the HTTP server if it is running.
     *
     * This method can be called from another thread to break out of the blocking Start() call.
     */
    void Stop();

private:
    std::shared_ptr<RequestExecutor> executor_;      ///< Shared pointer to the request executor.
    std::unique_ptr<ThreadPool> pool_;               ///< Unique pointer to the thread pool for handling requests.
    std::unique_ptr<httplib::Server> svr_;           ///< Unique pointer to the HTTP server instance.
};