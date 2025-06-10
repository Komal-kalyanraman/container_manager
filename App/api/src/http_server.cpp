/**
 * @file http_server.cpp
 * @brief Implements the HttpServerHandler class for handling HTTP requests.
 */

#include "inc/http_server.hpp"
#include <nlohmann/json.hpp>
#include "inc/logging.hpp"

/**
 * @brief Constructs an HttpServerHandler with the given executor and thread pool size.
 * @param thread_pool_size Number of threads in the thread pool for handling requests.
 * @param executor Shared pointer to a RequestExecutor for processing requests.
 */
HttpServerHandler::HttpServerHandler(int thread_pool_size, std::shared_ptr<RequestExecutor> executor)
    : pool_(std::make_unique<ThreadPool>(thread_pool_size)), executor_(std::move(executor)) {}

/**
 * @brief Starts the HTTP server on the specified host and port.
 *
 * Registers a POST handler for "/execute" that immediately acknowledges the request
 * and asynchronously dispatches it to the thread pool for processing.
 * This method blocks until the server is stopped (e.g., by calling Stop()).
 *
 * @param host The host address of the HTTP server.
 * @param port The port on which the server will listen for incoming requests.
 */
void HttpServerHandler::Start(const std::string& host, int port) {
    svr_ = std::make_unique<httplib::Server>();
    svr_->Post("/execute", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            // Log the incoming request
            CM_LOG_INFO << "[HTTP] Received request: " << req.body << std::endl;

            // Send immediate acknowledgment to the client
            nlohmann::json ack = {{"status", "success"}, {"message", "Request received and will be processed."}};
            res.set_content(ack.dump(), "application/json");

            // Process the request asynchronously in the thread pool
            std::string data = req.body;
            std::shared_ptr<RequestExecutor> exec = executor_; // capture for thread safety
            pool_->Enqueue([exec, data]() {
                try {
                    auto result = exec->Execute(data);
                    // Log the result status
                    if (result.contains("status")) {
                        if (result["status"] == "error") {
                            CM_LOG_ERROR << "[HTTP] Execution error: "
                                         << result.value("message", "Unknown error") << std::endl;
                        } else {
                            CM_LOG_INFO << "[HTTP] Execution success." << std::endl;
                        }
                    } else {
                        CM_LOG_WARN << "[HTTP] No status field in execution result." << std::endl;
                    }
                } catch (const std::exception& e) {
                    CM_LOG_ERROR << "[HTTP] Error executing request: " << e.what() << std::endl;
                }
            });
        } catch (const std::exception& e) {
            // Handle errors and send error response to client
            CM_LOG_ERROR << "[HTTP] Exception in request handler: " << e.what() << std::endl;
            nlohmann::json resp = {
                {"status", "error"},
                {"message", e.what()}
            };
            res.status = 400;
            res.set_content(resp.dump(), "application/json");
        }
    });
    // Start listening on the specified host and port (blocking call)
    CM_LOG_INFO << "[HTTP] Server listening on " << host << ":" << port << std::endl;
    svr_->listen(host, port);
}

/**
 * @brief Stops the HTTP server if it is running.
 *
 * This method can be called from another thread to break out of the blocking Start() call.
 */
void HttpServerHandler::Stop() {
    if (svr_) {
        CM_LOG_INFO << "[HTTP] Server stopping...";
        svr_->stop();
    }
}