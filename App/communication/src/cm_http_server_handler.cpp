#include "inc/cm_http_server_handler.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

HttpServerHandler::HttpServerHandler(std::shared_ptr<RequestExecutor> executor)
    : executor_(std::move(executor)), pool_(std::make_unique<ThreadPool>(kMaxThreadPoolSize)) {}

void HttpServerHandler::Start(int port) {
    httplib::Server svr;
    svr.Post("/execute", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            nlohmann::json ack = {{"status", "success"}, {"message", "Request received and will be processed."}};
            res.set_content(ack.dump(), "application/json");

            // Process in background
            std::string data = req.body;
            std::shared_ptr<RequestExecutor> exec = executor_; // capture for thread safety
            pool_->Enqueue([exec, data]() {
                try {
                    exec->Execute(data);
                } catch (const std::exception& e) {
                    // Optionally log error
                }
            });
        } catch (const std::exception& e) {
            nlohmann::json resp = {{"status", "error"}, {"message", e.what()}};
            res.status = 400;
            res.set_content(resp.dump(), "application/json");
        }
    });
    svr.listen(kHttpServerHost, port);
}