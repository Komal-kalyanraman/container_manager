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

// Forward declarations for dependency injection
class IDatabaseHandler;
class ContainerServiceHandler;
class ISecurityProvider;

/**
 * @class ProtoRequestExecutorHandler
 * @brief Parses and executes Protobuf requests, invoking business logic and database operations.
 *
 * This class is responsible for:
 *   - Receiving serialized Protobuf (or encrypted Protobuf) payloads from protocol handlers.
 *   - Detecting and decrypting encrypted payloads if necessary.
 *   - Deserializing the Protobuf request.
 *   - Transforming and saving the request to the database.
 *   - Invoking the container service handler to perform the requested operation.
 *   - Returning the result as a JSON object.
 *
 * Used by all protocol consumers (REST, MQTT, MQ, D-Bus) for Protobuf data format.
 */
class ProtoRequestExecutorHandler : public RequestExecutor {
public:
    /**
     * @brief Constructs a ProtoRequestExecutorHandler with injected dependencies.
     * @param db Reference to an IDatabaseHandler implementation.
     * @param service Reference to a ContainerServiceHandler.
     * @param security_provider Reference to a security provider for decryption.
     */
    ProtoRequestExecutorHandler(
        IDatabaseHandler& db, 
        ContainerServiceHandler& service,
        ISecurityProvider& security_provider
    );

    /**
     * @brief Receives a serialized protobuf string, deserializes, saves to DB, and returns result as JSON.
     *        Handles decryption if the payload is encrypted.
     * @param proto_data The input data as a serialized Protobuf string (or encrypted data).
     * @return A JSON object containing the result of the execution.
     */
    nlohmann::json Execute(const std::string& proto_data) override;

private:
    IDatabaseHandler& db_;               ///< Reference to the injected database handler.
    ContainerServiceHandler& service_;   ///< Reference to the injected service handler.
    ISecurityProvider& security_provider_; ///< Reference to the injected security provider.

    /**
     * @brief Helper method to detect if the input data is encrypted.
     *        Tries to parse as Protobuf and checks for expected fields.
     * @param data The input data string.
     * @return True if the data is likely encrypted, false if it is plain Protobuf.
     */
    bool IsEncryptedData(const std::string& data);
};