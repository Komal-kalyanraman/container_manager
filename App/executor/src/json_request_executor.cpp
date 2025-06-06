/**
 * @file json_request_executor.cpp
 * @brief Implementation of the JSON request executor for Container Manager.
 *
 * This file defines the logic for handling incoming JSON (or encrypted JSON) requests,
 * including decryption, parsing, validation, transformation, database storage,
 * and dispatching to the container service handler.
 */

#include "inc/json_request_executor.hpp"
#include <nlohmann/json.hpp>
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/redis_database.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"
#include "inc/security_provider.hpp"
#include <iostream>

using json = nlohmann::json;

/**
 * @brief Constructor for JsonRequestExecutorHandler.
 * @param db Reference to the database handler.
 * @param service Reference to the container service handler.
 * @param security_provider Reference to the security provider for decryption.
 */
JsonRequestExecutorHandler::JsonRequestExecutorHandler(
    IDatabaseHandler& db, 
    ContainerServiceHandler& service,
    ISecurityProvider& security_provider
)
    : db_(db), service_(service), security_provider_(security_provider) {}

/**
 * @brief Detects if the input data is encrypted.
 *        Tries to parse as JSON and checks for expected fields.
 * @param data The input data string.
 * @return True if the data is likely encrypted, false if it is plain JSON.
 */
bool JsonRequestExecutorHandler::IsEncryptedData(const std::string& data) {
    if (data.empty()) return false;

    // Try to parse as JSON first
    try {
        auto test_json = nlohmann::json::parse(data);
        // If parsing succeeds and we have expected fields, it's unencrypted JSON
        if (test_json.contains("runtime") || test_json.contains("operation")) {
            std::cout << "[JSON Executor] Detected valid unencrypted JSON data" << std::endl;
            return false;
        }
    } catch (const nlohmann::json::parse_error&) {
        // Parsing failed, likely encrypted
    }

    // If parsing fails or JSON is missing expected fields, assume encrypted
    std::cout << "[JSON Executor] Data doesn't parse as valid JSON - assuming encrypted" << std::endl;
    return true;
}

/**
 * @brief Executes a request represented as a JSON string.
 *        Handles decryption if the payload is encrypted, parses and validates the JSON,
 *        transforms and saves the request to the database, and invokes the container service handler.
 * @param data The input data as a JSON string (or encrypted data).
 * @return A JSON object containing the result of the execution.
 */
nlohmann::json JsonRequestExecutorHandler::Execute(const std::string& data) {
    try {
        std::string json_str;

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
        // Improved encryption detection
        if (IsEncryptedData(data)) {
            // Data appears to be encrypted - attempt decryption
            if (!security_provider_.Decrypt(data, json_str)) {
                std::cerr << "[Executor] Decryption failed for encrypted data" << std::endl;
                Status error_status = Status::Error(StatusCode::InvalidArgument, "Decryption failed");
                return {
                    {"status", "error"},
                    {"code", static_cast<int>(error_status.code)},
                    {"message", error_status.message}
                };
            }
            std::cout << "[Executor] Successfully decrypted data" << std::endl;
        } else {
            // Data appears to be plain JSON
            json_str = data;
            std::cout << "[Executor] Processing unencrypted JSON data" << std::endl;
        }
#else
        // Encryption disabled - treat all data as plain JSON
        json_str = data;
        std::cout << "[Executor] Processing data (encryption disabled)" << std::endl;
#endif

        // Parse JSON
        json j = json::parse(json_str);

        // Transform and save to database
        json transformed;
        transformed["runtime"] = j.value("runtime", "");
        transformed["status"] = kEmptyString;
        transformed["parameters"] = j.value("parameters", json::array());
        if (!transformed["parameters"].empty() && transformed["parameters"][0].is_object()) {
            transformed["parameters"][0].erase("container_name");
        }
        std::string key = j["parameters"][0].value("container_name", "unknown_container");
        db_.SaveJson(key, transformed);

        // Create container request
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
        
    } catch (const json::parse_error& e) {
        std::cerr << "[Executor] JSON parsing failed: " << e.what() << std::endl;
        Status error_status = Status::Error(StatusCode::InvalidArgument, "JSON parsing failed");
        return {
            {"status", "error"},
            {"code", static_cast<int>(error_status.code)},
            {"message", error_status.message}
        };
    } catch (const std::exception& e) {
        std::cerr << "[Executor] Execution failed: " << e.what() << std::endl;
        Status error_status = Status::Error(StatusCode::InvalidArgument, e.what());
        return {
            {"status", "error"},
            {"code", static_cast<int>(error_status.code)},
            {"message", error_status.message}
        };
    }
}