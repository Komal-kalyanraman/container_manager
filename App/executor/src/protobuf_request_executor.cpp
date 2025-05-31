/**
 * @file protobuf_request_executor.cpp
 * @brief Protocol Buffers request executor for Container Manager
 *
 * Implements ProtoRequestExecutorHandler for processing serialized protobuf data,
 * including optional AES-GCM decryption and integration with the service handler.
 */

#include "inc/protobuf_request_executor.hpp"
#include "container_manager.pb.h"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"
#include "inc/aes_gcm_decrypt.hpp"
#include <nlohmann/json.hpp>

/**
 * @brief Constructs a ProtoRequestExecutorHandler with database and service handler.
 */
ProtoRequestExecutorHandler::ProtoRequestExecutorHandler(IDatabaseHandler& db, ContainerServiceHandler& service)
    : db_(db), service_(service) {}

/**
 * @brief Execute container management request from serialized protobuf data.
 *        Handles decryption (if enabled), deserialization, and operation execution.
 * @param proto_data Serialized protobuf data (binary) or encrypted payload.
 * @return JSON object containing operation result or error information.
 */
nlohmann::json ProtoRequestExecutorHandler::Execute(const std::string& proto_data) {
    try {
#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
        std::string proto_plain;
        if (!decrypt_ota_payload(proto_data, proto_plain)) {
            Status error_status = Status::Error(StatusCode::InvalidArgument, "Decryption failed");
            return {
                {"status", "error"},
                {"code", static_cast<int>(error_status.code)},
                {"message", error_status.message}
            };
        }
        const std::string& proto_input = proto_plain;
#else
        const std::string& proto_input = proto_data;
#endif

        containermanager::ContainerRequest proto_req;
        if (!proto_req.ParseFromString(proto_input)) {
            Status error_status = Status::Error(StatusCode::InvalidArgument, "Failed to parse protobuf");
            return {
                {"status", "error"},
                {"code", static_cast<int>(error_status.code)},
                {"message", error_status.message}
            };
        }

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

        std::string key = "unknown_container";
        if (!proto_req.parameters().empty()) {
            key = proto_req.parameters(0).container_name();
            transformed["parameters"][0].erase("container_name");
        }
        db_.SaveJson(key, transformed);

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
        }

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