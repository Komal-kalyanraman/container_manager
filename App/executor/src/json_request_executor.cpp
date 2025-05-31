/**
 * @file json_request_executor.cpp
 * @brief JSON request executor for Container Manager
 */

#include "inc/json_request_executor.hpp"
#include <nlohmann/json.hpp>
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/redis_database.hpp"
#include "inc/aes_gcm_decrypt.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"
#include <iostream>

using json = nlohmann::json;

JsonRequestExecutorHandler::JsonRequestExecutorHandler(IDatabaseHandler& db, ContainerServiceHandler& service)
    : db_(db), service_(service) {}

/**
 * @brief Execute container management request from JSON string or encrypted payload.
 * @param data JSON string data or encrypted payload.
 * @return JSON object containing operation result or error information.
 */
nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    try {
        std::string json_str;

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
        bool is_encrypted = !data.empty() && data[0] != '{' && data[0] != '[';
        if (is_encrypted) {
            if (!decrypt_ota_payload(data, json_str)) {
                Status error_status = Status::Error(StatusCode::InvalidArgument, "Decryption failed");
                return {
                    {"status", "error"},
                    {"code", static_cast<int>(error_status.code)},
                    {"message", error_status.message}
                };
            }
        } else {
            json_str = data;
        }
#else
        json_str = data;
#endif

        json j = json::parse(json_str);

        json transformed;
        transformed["runtime"] = j.value("runtime", "");
        transformed["status"] = kEmptyString;
        transformed["parameters"] = j.value("parameters", json::array());
        if (!transformed["parameters"].empty() && transformed["parameters"][0].is_object()) {
            transformed["parameters"][0].erase("container_name");
        }
        std::string key = j["parameters"][0].value("container_name", "unknown_container");
        db_.SaveJson(key, transformed);

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