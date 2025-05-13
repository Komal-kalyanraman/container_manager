#include "inc/cm_server_handler.hpp"

#include <iostream>

#include <httplib.h>
#include <nlohmann/json.hpp>

#include "inc/cm_common.hpp"
#include "inc/cm_logging.hpp"
#include "inc/cm_docker_commands.hpp"
#include "inc/cm_docker_api_commands.hpp"
#include "inc/cm_podman_commands.hpp"
#include "inc/cm_podman_api_commands.hpp"
#include "inc/cm_command_factory.hpp"

using json = nlohmann::json;

ServerHandler& ServerHandler::GetInstance()
{
    static ServerHandler self{};
    return self;
}

void ServerHandler::ServerMainLoop() {
    httplib::Server svr;

    Invoker invoker;

    svr.Post("/execute", [&invoker](const httplib::Request& req, httplib::Response& res) {
        try {
            json data = json::parse(req.body);
            std::cout << "Received JSON data:\n";
            std::cout << "runtime: " << data.value("runtime", "") << std::endl;

            std::string runtime = data["runtime"];
            RuntimeType runtime_type;

            if(runtime == "docker" || runtime == "Docker" || runtime == "DOCKER") {
                runtime_type = RuntimeType::Docker;
                CM_LOG_INFO << "Docker runtime selected";
            } else if (runtime == "docker-api" || runtime == "DockerApi" || runtime == "DOCKERAPI" || runtime == "dockerapi") {
                runtime_type = RuntimeType::DockerApi;
                CM_LOG_INFO << "Docker API runtime selected";
            } else if(runtime == "podman" || runtime == "Podman" || runtime == "PODMAN") {
                runtime_type = RuntimeType::Podman;
                CM_LOG_INFO << "Podman runtime selected";
            } else if (runtime == "podman-api" || runtime == "PodmanApi" || runtime == "PODMANAPI" || runtime == "podmanapi") {
                runtime_type = RuntimeType::PodmanApi;
                CM_LOG_INFO << "Podman API runtime selected";
            } else {
                CM_LOG_FATAL << "Invalid runtime type";
                json resp = {{"status", "error"}, {"message", "Invalid runtime type"}};
                res.status = 400;
                res.set_content(resp.dump(), "application/json");
                return;
            }

            invoker.SetCommand(CommandFactory::CreateCommand(runtime_type, CommandType::RuntimeStatus));
            if (!invoker.Invoke()) {
                json resp = {{"status", "error"}, {"message", "Selected runtime is not running"}};
                res.status = 500;
                res.set_content(resp.dump(), "application/json");
                return;
            }

            auto parameters = data.value("parameters", json::array());
            if (!parameters.empty() && parameters[0].is_object()) {
                const auto& param = parameters[0];
                CM_LOG_INFO << "container_name: " << param.value("container_name", "");
                CM_LOG_INFO << "cpus: " << param.value("cpus", "");
                CM_LOG_INFO << "memory: " << param.value("memory", "");
                CM_LOG_INFO << "pids: " << param.value("pids", "");
                CM_LOG_INFO << "restart_policy: " << param.value("restart_policy", "");
                CM_LOG_INFO << "image_name: " << param.value("image_name", "");
                std::string container_name = param.value("container_name", "");
                std::string image_name = param.value("image_name", "");
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