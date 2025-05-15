#include "inc/json_request_executor.hpp"

#include <nlohmann/json.hpp>

#include "inc/database_interface.hpp"
#include "inc/redis_database.hpp"
#include "inc/container_service.hpp"

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

    // Use the database interface (can be swapped for any backend)
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string key = j["parameters"][0].value("container_name", "unknown_container");
    db.SaveJson(key, transformed);

    // Continue with your business logic if needed
    ContainerRequest req;
    req.operation = j.value("operation", "");
    req.runtime = j.value("runtime", "");
    if (!j["parameters"].empty()) {
        const auto& param = j["parameters"][0];
        req.container_name = param.value("container_name", "");
    }
    return ContainerServiceHandler::HandleRequest(req);
}