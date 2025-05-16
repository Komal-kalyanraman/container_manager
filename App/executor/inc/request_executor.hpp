/**
 * @file request_executor.hpp
 * @brief Declares the abstract RequestExecutor interface for executing requests.
 */

#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "inc/container_request.hpp"

/// @class RequestExecutor
/// @brief Abstract base class for request executors supporting different input formats.
class RequestExecutor {
public:
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~RequestExecutor() = default;

    /**
     * @brief Executes a request and returns a JSON response.
     * @param data The input data as a string (could be JSON, Protobuf, etc.).
     * @return A JSON object containing the result of the execution.
     */
    virtual nlohmann::json Execute(const std::string& data) = 0;
};