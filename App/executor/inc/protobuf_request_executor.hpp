/**
 * @file protobuf_request_executor.hpp
 * @brief Declares the ProtoRequestExecutorHandler class for handling Protobuf-based requests.
 *
 * This executor receives serialized Protobuf data as a string, deserializes it into a ContainerRequest,
 * saves the request to the database, and returns the result as a JSON object.
 * The class uses dependency injection for both the database handler and the service handler.
 * It implements the RequestExecutor interface and is used by all protocol consumers
 * (HTTP, MQTT, Message Queue, D-Bus) when Protobuf is selected as the data format.
 */

#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "inc/request_executor.hpp"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp" 

// Forward declarations
class IDatabaseHandler;
class ContainerServiceHandler;
class ISecurityProvider;

/**
 * @class ProtoRequestExecutorHandler
 * @brief Parses and executes Protobuf requests, invoking business logic and database operations.
 *
 * Receives a serialized Protobuf string, deserializes it, transforms and saves it to the database,
 * and dispatches the request to the container service handler. Returns the result as a JSON object.
 */
class ProtoRequestExecutorHandler : public RequestExecutor {
public:
    ProtoRequestExecutorHandler(
        IDatabaseHandler& db, 
        ContainerServiceHandler& service,
        ISecurityProvider& security_provider
    );

    /**
     * @brief Receives a serialized protobuf string, deserializes, saves to DB, and returns result as JSON.
     * @param proto_data The input data as a serialized Protobuf string.
     * @return A JSON object containing the result of the execution.
     */
    nlohmann::json Execute(const std::string& proto_data) override;

private:
    IDatabaseHandler& db_;               ///< Reference to the injected database handler.
    ContainerServiceHandler& service_;   ///< Reference to the injected service handler.

    ISecurityProvider& security_provider_;
    
    // Helper method to detect if data is encrypted
    bool IsEncryptedData(const std::string& data);
};