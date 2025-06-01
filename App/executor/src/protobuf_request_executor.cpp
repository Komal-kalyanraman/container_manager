/**
 * @file protobuf_request_executor.cpp
 * @brief Protocol Buffers request executor for Container Manager
 */

#include "inc/protobuf_request_executor.hpp"
#include "container_manager.pb.h"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp"
#include "inc/common.hpp"
#include "inc/status.hpp"
#include "inc/security_provider.hpp"
#include <nlohmann/json.hpp>
#include <iostream>

ProtoRequestExecutorHandler::ProtoRequestExecutorHandler(
    IDatabaseHandler& db, 
    ContainerServiceHandler& service,
    ISecurityProvider& security_provider
)
    : db_(db), service_(service), security_provider_(security_provider) {}

bool ProtoRequestExecutorHandler::IsEncryptedData(const std::string& data) {
    if (data.empty()) return false;
    
    // Strategy: Try to parse as protobuf first
    // If parsing succeeds and we get valid fields, it's unencrypted protobuf
    // If parsing fails, it's likely encrypted data
    
    containermanager::ContainerRequest test_proto;
    
    // Attempt to parse the data as protobuf
    if (test_proto.ParseFromString(data)) {
        // Parsing succeeded - check if it has meaningful content
        // Valid protobuf should have at least runtime or operation fields
        if (!test_proto.runtime().empty() || !test_proto.operation().empty()) {
            std::cout << "[Proto Executor] Detected valid unencrypted protobuf data" << std::endl;
            return false;  // Valid protobuf structure, not encrypted
        }
    }
    
    // If we reach here, either parsing failed or the parsed proto is empty/invalid
    // This suggests the data is encrypted
    std::cout << "[Proto Executor] Data doesn't parse as valid protobuf - assuming encrypted" << std::endl;
    return true;
}

nlohmann::json ProtoRequestExecutorHandler::Execute(const std::string& proto_data) {
    try {
        std::string proto_input;

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
        // Use protobuf-specific encryption detection
        if (IsEncryptedData(proto_data)) {
            // Data appears to be encrypted - attempt decryption
            if (!security_provider_.Decrypt(proto_data, proto_input)) {
                std::cerr << "[Proto Executor] Decryption failed for encrypted data" << std::endl;
                Status error_status = Status::Error(StatusCode::InvalidArgument, "Decryption failed");
                return {
                    {"status", "error"},
                    {"code", static_cast<int>(error_status.code)},
                    {"message", error_status.message}
                };
            }
            std::cout << "[Proto Executor] Successfully decrypted protobuf data" << std::endl;
        } else {
            // Data appears to be plain protobuf (already validated in IsEncryptedData)
            proto_input = proto_data;
            std::cout << "[Proto Executor] Processing unencrypted protobuf data" << std::endl;
        }
#else
        // Encryption disabled - treat all data as plain protobuf
        proto_input = proto_data;
        std::cout << "[Proto Executor] Processing data (encryption disabled)" << std::endl;
#endif

        // Parse protobuf (this might be redundant if we already parsed in IsEncryptedData, 
        // but it's safer to parse again with the final data)
        containermanager::ContainerRequest proto_req;
        if (!proto_req.ParseFromString(proto_input)) {
            std::cerr << "[Proto Executor] Protobuf parsing failed on final data" << std::endl;
            Status error_status = Status::Error(StatusCode::InvalidArgument, "Failed to parse protobuf");
            return {
                {"status", "error"},
                {"code", static_cast<int>(error_status.code)},
                {"message", error_status.message}
            };
        }

        // Validate that we have meaningful data
        if (proto_req.runtime().empty() && proto_req.operation().empty()) {
            std::cerr << "[Proto Executor] Parsed protobuf has no meaningful content" << std::endl;
            Status error_status = Status::Error(StatusCode::InvalidArgument, "Invalid protobuf content");
            return {
                {"status", "error"},
                {"code", static_cast<int>(error_status.code)},
                {"message", error_status.message}
            };
        }

        // Transform and save to database
        nlohmann::json transformed;
        transformed["runtime"] = proto_req.runtime();
        transformed["status"] = kEmptyString;
        transformed["parameters"] = nlohmann::json::array();

        for (const auto& param : proto_req.parameters()) {
            nlohmann::json param_json;
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
        }
        db_.SaveJson(key, transformed);

        // Create container request
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
        std::cerr << "[Proto Executor] Execution failed: " << e.what() << std::endl;
        Status error_status = Status::Error(StatusCode::InternalError, e.what());
        return {
            {"status", "error"},
            {"code", static_cast<int>(error_status.code)},
            {"message", error_status.message}
        };
    }
}