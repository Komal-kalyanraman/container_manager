#include "inc/cm_server_handler.hpp"

#include <httplib.h>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

ServerHandler& ServerHandler::GetInstance()
{
    static ServerHandler self{};
    return self;
}

void ServerHandler::ServerMainLoop() {
    httplib::Server svr;

    svr.Post("/", [](const httplib::Request& req, httplib::Response& res) {
        try {
            auto data = json::parse(req.body);
            std::cout << "Received JSON data:\n";
            std::cout << "type: " << data.value("type", "") << std::endl;
            auto parameters = data.value("parameters", json::array());
            if (!parameters.empty() && parameters[0].is_object()) {
                const auto& param = parameters[0];
                std::cout << "container_name: " << param.value("container_name", "") << std::endl;
                std::cout << "cpus: " << param.value("cpus", "") << std::endl;
                std::cout << "memory: " << param.value("memory", "") << std::endl;
                std::cout << "pids: " << param.value("pids", "") << std::endl;
                std::cout << "restart_policy: " << param.value("restart_policy", "") << std::endl;
                std::cout << "image_name: " << param.value("image_name", "") << std::endl;
            }
            json resp = {{"status", "success"}, {"message", "Data received"}};
            res.set_content(resp.dump(), "application/json");
        } catch (const std::exception& e) {
            json resp = {{"status", "error"}, {"message", e.what()}};
            res.status = 400;
            res.set_content(resp.dump(), "application/json");
        }
    });

    // Run the server (blocking call, runs as long as process is alive)
    svr.listen("0.0.0.0", 5000);
}

void ServerHandler::StartCmServer() {
    // Start the server in a dedicated thread; thread runs for process lifetime
    server_thread_ = std::thread(&ServerHandler::ServerMainLoop, this);
    server_thread_.detach(); // Detach since we don't plan to join/stop it
}