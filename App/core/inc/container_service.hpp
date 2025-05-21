/**
 * @file container_service.hpp
 * @brief Declares the ContainerServiceHandler class for business logic related to containers.
 */

#pragma once

#include <nlohmann/json.hpp>
#include "inc/container_request.hpp"

/// @class ContainerServiceHandler
/// @brief Provides static methods for handling container operations and runtime checks.
/// @details This class acts as a service layer, coordinating container lifecycle operations and runtime status checks.
class ContainerServiceHandler {
public:
    /// @brief Handles a container request by dispatching to the appropriate operation.
    /// @param req The container request containing operation, runtime, and container details.
    /// @return A JSON object with the result of the operation.
    static nlohmann::json HandleRequest(const ContainerRequest& req);

    /// @brief Performs the specified container operation using the request structure.
    /// @param req The container request containing all necessary details.
    /// @return True if the operation succeeded, false otherwise.
    static bool ContainerOperations(const ContainerRequest& req);
};