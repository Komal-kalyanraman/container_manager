/**
 * @file http_server.hpp
 * @brief Defines the HttpServerHandler class for handling HTTP requests.
 */

#pragma once
#include <memory>

#include "inc/common.hpp"
#include "inc/threadpool.hpp"
#include "inc/json_request_executor.hpp"

/// @class HttpServerHandler
/// @brief Handles incoming HTTP requests and dispatches them to the appropriate executor.
/// @details This class manages the HTTP server lifecycle and uses a thread pool to process requests concurrently.
class HttpServerHandler {
public:
    /// @brief Constructs an HTTP server handler.
    /// @param executor Shared pointer to a RequestExecutor for processing requests.
    explicit HttpServerHandler(std::shared_ptr<RequestExecutor> executor);

    /// @brief  Starts the HTTP server on the specified port.
    /// @param port The port on which the server will listen for incoming requests.
    void Start(int port);

private:
    /// @brief Handles incoming HTTP requests.
    std::shared_ptr<RequestExecutor> executor_;

    /// @brief Thread pool for handling requests concurrently.
    std::unique_ptr<ThreadPool> pool_;
};