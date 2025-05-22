/**
 * @file protobuf_request_executor.cpp
 * @brief Implements the ProtoRequestExecutorHandler class for handling Protobuf-based requests.
 *
 * This file defines the logic for receiving serialized Protobuf data, deserializing it into a ContainerRequest,
 * converting it to JSON for database storage, and invoking the container service handler.
 * The result is returned as a JSON object.
 *
 * Used by all protocol consumers (HTTP, MQTT, Message Queue, D-Bus) when Protobuf is selected as the data format.
 */

#include "inc/protobuf_request_executor.hpp"
#include "container_manager.pb.h"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/redis_database.hpp"
#include "inc/common.hpp"
#include <nlohmann/json.hpp>

/**
 * @brief Default constructor for ProtoRequestExecutorHandler.
 */
ProtoRequestExecutorHandler::ProtoRequestExecutorHandler() = default;

/**
 * @brief Executes a request represented as a serialized Protobuf string.
 *        Deserializes the input, transforms and saves it to the database, and dispatches to the service handler.
 * @param proto_data The input data as a serialized Protobuf string.
 * @return A JSON object containing the result of the execution.
 */
nlohmann::json ProtoRequestExecutorHandler::Execute(const std::string& proto_data) {
    containermanager::ContainerRequest proto_req;
    if (!proto_req.ParseFromString(proto_data)) {
        // Return error as JSON
        return {{"status", "error"}, {"message", "Failed to parse protobuf"}};
    }

    // Convert proto to JSON for DB storage
    nlohmann::json transformed;
    transformed["runtime"] = proto_req.runtime();
    transformed["status"] = kEmptyString;
    transformed["parameters"] = nlohmann::json::array();

    for (const auto& param : proto_req.parameters()) {
        nlohmann::json param_json;
        param_json["container_name"] = param.container_name();
        param_json["cpus"] = param.cpus();
        param_json["memory"] = param.memory();
        param_json["pids"] = param.pids();
        param_json["restart_policy"] = param.restart_policy();
        param_json["image_name"] = param.image_name();
        transformed["parameters"].push_back(param_json);
    }

    // Save to DB (use first parameter's container_name as key if available)
    std::string key = "unknown_container";
    if (!proto_req.parameters().empty()) {
        key = proto_req.parameters(0).container_name();
        // Remove "container_name" from parameters[0] for DB storage, as in JSON executor
        transformed["parameters"][0].erase("container_name");
    }
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    db.SaveJson(key, transformed);

    // Fill internal ContainerRequest struct
    ContainerRequest req;
    req.runtime = proto_req.runtime();
    req.operation = proto_req.operation();
    if (!proto_req.parameters().empty()) {
        const auto& param = proto_req.parameters(0);
        req.container_name = param.container_name();
        req.image_name = param.image_name();
        // Add more fields here if needed in the future
    }

    // Call business logic and return result
    return ContainerServiceHandler::HandleRequest(req);
}