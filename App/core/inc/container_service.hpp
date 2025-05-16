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
    
    /**
     * @brief Checks if the specified container runtime is available for the given operation.
     * @param runtime The container runtime (e.g., "docker", "podman").
     * @param operation The operation to check runtime available.
     * @return True if the runtime is available, false otherwise.
     */
    static bool CheckRuntimeAvailable(const std::string& runtime, const std::string& operation);

    /**
     * @brief Performs the specified container operation.
     * @param runtime The container runtime (e.g., "docker", "podman").
     * @param operation The operation to perform (e.g., "create", "start", "stop").
     * @param container_name The name of the container to operate on.
     * @return True if the operation succeeded, false otherwise.
     */
    static bool ContainerOperations(const std::string& runtime, const std::string& operation, const std::string& container_name);
};