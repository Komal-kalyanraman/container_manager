/**
 * @file json_request_executor.hpp
 * @brief Defines the JsonRequestExecutorHandler class for executing JSON-based requests.
 */

#pragma once

#include "inc/request_executor.hpp"

/// @class JsonRequestExecutorHandler
/// @brief Parses and executes JSON requests, invoking business logic and database operations.
class JsonRequestExecutorHandler : public RequestExecutor {
public:
    /**
     * @brief Executes a request represented as a JSON string.
     * @param data The input data as a JSON string.
     * @return A JSON object containing the result of the execution.
     */
    nlohmann::json Execute(const std::string& data) override;
};