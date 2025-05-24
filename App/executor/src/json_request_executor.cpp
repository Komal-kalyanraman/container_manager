/**
 * @file json_request_executor.cpp
 * @brief Implements the JsonRequestExecutorHandler class for executing JSON-based requests.
 */

#include "inc/json_request_executor.hpp"

#include <nlohmann/json.hpp>
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/redis_database.hpp"
#include "inc/common.hpp"

using json = nlohmann::json;
#include <iostream>

/**
 * @brief Executes a request represented as a JSON string.
 *        Transforms the input, saves it to the database, and dispatches to the service handler.
 * @param data The input data as a JSON string.
 * @return A JSON object containing the result of the execution.
 */
nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    json j = json::parse(data);

    // Transform JSON as requested
    json transformed;
    transformed["runtime"] = j.value("runtime", "");
    transformed["status"] = kEmptyString;
    transformed["parameters"] = j.value("parameters", json::array());
    if (!transformed["parameters"].empty() && transformed["parameters"][0].is_object()) {
        // Remove "container_name" from parameters[0]
        transformed["parameters"][0].erase("container_name");
    }

    // Use the database interface (can be swapped for any backend)
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string key = j["parameters"][0].value("container_name", "unknown_container");
    db.SaveJson(key, transformed);

    // Prepare the container request for business logic
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
        // Add more fields here if needed in the future
    }
    return ContainerServiceHandler::HandleRequest(req);
}