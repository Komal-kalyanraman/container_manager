#include "inc/cm_http_server_handler.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>

HttpServerHandler::HttpServerHandler(std::shared_ptr<RequestExecutor> executor)
    : executor_(std::move(executor)) {}

void HttpServerHandler::Start(int port) {
    httplib::Server svr;
    svr.Post("/execute", [this](const httplib::Request& req, httplib::Response& res) {
        try {
            auto response = executor_->Execute(req.body);
            res.set_content(response.dump(), "application/json");
        } catch (const std::exception& e) {
            nlohmann::json resp = {{"status", "error"}, {"message", e.what()}};
            res.status = 400;
            res.set_content(resp.dump(), "application/json");
        }
    });
    svr.listen(kHttpServerHost, port);
}