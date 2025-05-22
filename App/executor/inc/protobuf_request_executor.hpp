/**
 * @file protobuf_request_executor.hpp
 * @brief Defines the ProtoRequestExecutorHandler class for handling Protobuf-based requests.
 *
 * This executor receives serialized Protobuf data as a string, deserializes it into a ContainerRequest,
 * saves the request to the database, and returns the result as a JSON object.
 * It implements the RequestExecutor interface and is used by all protocol consumers
 * (HTTP, MQTT, Message Queue, D-Bus) when Protobuf is selected as the data format.
 */
#pragma once

#include "inc/request_executor.hpp"
#include <string>
#include <nlohmann/json.hpp>

/**
 * @class ProtoRequestExecutorHandler
 * @brief Executes requests received as serialized Protobuf data.
 */
class ProtoRequestExecutorHandler : public RequestExecutor {
public:
    ProtoRequestExecutorHandler();
    ~ProtoRequestExecutorHandler() override = default;

    /**
     * @brief Receives a serialized protobuf string, deserializes, saves to DB, and returns result as JSON.
     * @param proto_data The input data as a serialized Protobuf string.
     * @return A JSON object containing the result of the execution.
     */
    nlohmann::json Execute(const std::string& proto_data) override;
};