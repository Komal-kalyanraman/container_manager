#include "inc/cm_json_requestexecutor_handler.hpp"

#include <nlohmann/json.hpp>

#include "inc/cm_container_service_handler.hpp"
#include "inc/cm_database_handler.hpp"

using json = nlohmann::json;
#include <iostream>

nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    // Parse JSON
    std::cout << "Received JSON data: " << data << std::endl;
    json j = json::parse(data);

    // Transform JSON as requested
    json transformed;
    transformed["runtime"] = j.value("runtime", "");
    transformed["status"] = j.value("operation", "");
    transformed["parameters"] = j.value("parameters", json::array());
    if (!transformed["parameters"].empty() && transformed["parameters"][0].is_object()) {
        // Remove "container_name" from parameters[0]
        transformed["parameters"][0].erase("container_name");
    }

    // Save to Redis using singleton database handler
    auto& db = DatabaseHandler::GetInstance();
    // Use a unique key, e.g., container_name or a UUID
    std::string key = j["parameters"][0].value("container_name", "unknown_container");
    db.SaveJson(key, transformed);

    // Continue with your business logic if needed
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
    return ContainerServiceHandler::HandleRequest(req);
}