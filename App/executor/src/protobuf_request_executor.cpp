/**
 * @file protobuf_request_executor.cpp
 * @brief Implements the ProtoRequestExecutorHandler class for handling Protobuf-based requests.
 *
 * This file defines the logic for receiving serialized Protobuf data, deserializing it into a ContainerRequest,
 * converting it to JSON for database storage, and invoking the container service handler.
 * The result is returned as a JSON object.
 *
 * Used by all protocol consumers (HTTP, MQTT, Message Queue, D-Bus) when Protobuf is selected as the data format.
 * Provides standardized error handling and logging using the Status structure.
 */

#include "inc/protobuf_request_executor.hpp"
#include "container_manager.pb.h"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"
#include <nlohmann/json.hpp>

/**
 * @brief Constructs a ProtoRequestExecutorHandler with a database handler and service handler.
 * @param db Reference to an IDatabaseHandler implementation.
 * @param service Reference to a ContainerServiceHandler.
 */
ProtoRequestExecutorHandler::ProtoRequestExecutorHandler(IDatabaseHandler& db, ContainerServiceHandler& service)
    : db_(db), service_(service) {}

/**
 * @brief Executes a request represented as a serialized Protobuf string.
 *        Deserializes the input, transforms and saves it to the database, and dispatches to the service handler.
 * @param proto_data The input data as a serialized Protobuf string.
 * @return A JSON object containing the result of the execution.
 *
 * Steps:
 * 1. Parse the protobuf string into a ContainerRequest proto object.
 * 2. Convert the proto parameters to a JSON object for database storage.
 * 3. Remove "container_name" from the first parameter for DB consistency.
 * 4. Save the transformed JSON to the database using the container name as the key.
 * 5. Fill an internal ContainerRequest struct for business logic.
 * 6. Call the service handler and return its result as JSON.
 * 7. On error, return a JSON object with status, code, and message fields.
 */
nlohmann::json ProtoRequestExecutorHandler::Execute(const std::string& proto_data) {
    try {
        containermanager::ContainerRequest proto_req;
        if (!proto_req.ParseFromString(proto_data)) {
            Status error_status = Status::Error(StatusCode::InvalidArgument, "Failed to parse protobuf");
            return {
                {"status", "error"},
                {"code", static_cast<int>(error_status.code)},
                {"message", error_status.message}
            };
        }

        // Convert proto to JSON for DB storage
        nlohmann::json transformed;
        transformed["runtime"] = proto_req.runtime();
        transformed["status"] = kEmptyString;
        transformed["parameters"] = nlohmann::json::array();

        // Convert each parameter from proto to JSON
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
        db_.SaveJson(key, transformed);

        // Fill internal ContainerRequest struct for business logic
        ContainerRequest req;
        req.runtime = proto_req.runtime();
        req.operation = proto_req.operation();
        if (!proto_req.parameters().empty()) {
            const auto& param = proto_req.parameters(0);
            req.container_name = param.container_name();
            req.image_name = param.image_name();
            req.cpus = param.cpus();
            req.memory = param.memory();
            req.pids = param.pids();
            req.restart_policy = param.restart_policy();
            // Add more fields here if needed in the future
        }

        // Call business logic and return result
        return service_.HandleRequest(req);
    } catch (const std::exception& e) {
        Status error_status = Status::Error(StatusCode::InternalError, e.what());
        return {
            {"status", "error"},
            {"code", static_cast<int>(error_status.code)},
            {"message", error_status.message}
        };
    }
}