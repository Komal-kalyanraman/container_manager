/**
 * @file json_request_executor.cpp
 * @brief Implements the JsonRequestExecutorHandler class for executing JSON-based requests.
 *
 * This file defines the logic for receiving JSON data, transforming it for database storage,
 * saving it to the database, and invoking the container service handler for business logic.
 * The result is returned as a JSON object.
 *
 * Used by all protocol consumers (HTTP, MQTT, Message Queue, D-Bus) when JSON is selected as the data format.
 * Provides standardized error handling and logging using the Status structure.
 */

#include "inc/json_request_executor.hpp"

#include <nlohmann/json.hpp>
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/redis_database.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"

using json = nlohmann::json;
#include <iostream>

/**
 * @brief Constructs a JsonRequestExecutorHandler with a database handler and service handler.
 * @param db Reference to an IDatabaseHandler implementation.
 * @param service Reference to a ContainerServiceHandler.
 */
JsonRequestExecutorHandler::JsonRequestExecutorHandler(IDatabaseHandler& db, ContainerServiceHandler& service)
    : db_(db), service_(service) {}

/**
 * @brief Executes a request represented as a JSON string.
 *        Transforms the input, saves it to the database, and dispatches to the service handler.
 * @param data The input data as a JSON string.
 * @return A JSON object containing the result of the execution.
 *
 * Steps:
 * 1. Parse the input JSON string.
 * 2. Transform the JSON for database storage (removing "container_name" from parameters[0]).
 * 3. Save the transformed JSON to the database using the container name as the key.
 * 4. Fill an internal ContainerRequest struct for business logic.
 * 5. Call the service handler and return its result as JSON.
 * 6. On error, return a JSON object with status, code, and message fields.
 */
nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    try {
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

        // Use the injected database interface
        std::string key = j["parameters"][0].value("container_name", "unknown_container");
        db_.SaveJson(key, transformed);

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
        return service_.HandleRequest(req);
    } catch (const std::exception& e) {
        Status error_status = Status::Error(StatusCode::InvalidArgument, e.what());
        return {
            {"status", "error"},
            {"code", static_cast<int>(error_status.code)},
            {"message", error_status.message}
        };
    }
}