/**
 * @file json_request_executor.cpp
 * @brief JSON request executor for Container Manager
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

JsonRequestExecutorHandler::JsonRequestExecutorHandler(
    IDatabaseHandler& db, 
    ContainerServiceHandler& service,
    ISecurityProvider& security_provider
)
    : db_(db), service_(service), security_provider_(security_provider) {}

bool JsonRequestExecutorHandler::IsEncryptedData(const std::string& data) {
    if (data.empty()) return false;
    
    // First check: If it starts with JSON characters, it's likely plain JSON
    char first_char = data[0];
    if (first_char == '{' || first_char == '[') {
        return false;
    }
    
    // Second check: Count non-printable characters
    int non_printable = 0;
    int sample_size = std::min(static_cast<int>(data.length()), 50); // Check first 50 chars
    
    for (int i = 0; i < sample_size; ++i) {
        unsigned char c = static_cast<unsigned char>(data[i]);
        // Consider characters outside printable ASCII range as non-printable
        if (c < 32 || c > 126) {
            non_printable++;
        }
    }
    
    // If more than 40% are non-printable, consider it encrypted
    double non_printable_ratio = static_cast<double>(non_printable) / sample_size;
    return non_printable_ratio > 0.4;
}

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