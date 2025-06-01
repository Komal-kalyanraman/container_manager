/**
 * @file json_request_executor.hpp
 * @brief Declares the JsonRequestExecutorHandler class for executing JSON-based requests.
 *
 * This executor receives JSON data as a string, parses it, transforms the request,
 * saves it to the database, and invokes the container service handler to perform
 * the requested operation. The result is returned as a JSON object.
 * 
 * The class uses dependency injection for both the database handler and the service handler.
 * It implements the RequestExecutor interface and is used by all protocol consumers
 * (HTTP, MQTT, Message Queue, D-Bus) when JSON is selected as the data format.
 */

#pragma once

#include "inc/request_executor.hpp"
#include "inc/database_interface.hpp"
#include "inc/container_service.hpp" 

// Forward declarations
class IDatabaseHandler;
class ContainerServiceHandler;
class ISecurityProvider;

/**
 * @class JsonRequestExecutorHandler
 * @brief Parses and executes JSON requests with optional encryption support
 */
class JsonRequestExecutorHandler : public RequestExecutor {
public:
    /**
     * @brief Constructs a JsonRequestExecutorHandler with injected dependencies.
     * @param db Reference to an IDatabaseHandler implementation.
     * @param service Reference to a ContainerServiceHandler.
     * @param security_provider Optional security provider for decryption.
     */
    JsonRequestExecutorHandler(
        IDatabaseHandler& db, 
        ContainerServiceHandler& service,
        ISecurityProvider& security_provider
    );

    /**
     * @brief Executes a request represented as a JSON string.
     * @param data The input data as a JSON string (or encrypted data).
     * @return A JSON object containing the result of the execution.
     */
    nlohmann::json Execute(const std::string& data) override;

private:
    IDatabaseHandler& db_;                   ///< Reference to the injected database handler.
    ContainerServiceHandler& service_;       ///< Reference to the injected service handler.
    ISecurityProvider& security_provider_;   ///< Reference to security provider.

    // Helper method to detect if data is encrypted
    bool IsEncryptedData(const std::string& data);
};