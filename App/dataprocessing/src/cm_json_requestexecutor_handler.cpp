#include "inc/cm_json_requestexecutor_handler.hpp"

#include <nlohmann/json.hpp>

#include "inc/cm_container_service_handler.hpp"

using json = nlohmann::json;

#include <iostream>

nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    // Parse JSON
    std::cout << "Received JSON data: " << data << std::endl;
    json j = json::parse(data);
    ContainerRequest req;
    req.operation = j.value("operation", "");
    req.runtime = j.value("runtime", "");
    if (!j["parameters"].empty()) {
        const auto& param = j["parameters"][0];
        req.container_name = param.value("container_name", "");
        req.image_name = param.value("image_name", "");
        req.cpus = param.value("cpus", "");
        req.memory = param.value("memory", "");
        req.pids = param.value("pids", "");
        req.restart_policy = param.value("restart_policy", "");
    }
    // Call business logic
    return ContainerServiceHandler::HandleRequest(req);
}