/**
 * @file container_request.hpp
 * @brief Data structure for representing a container operation request.
 */

#pragma once
#include <string>

/// @struct ContainerRequest
/// @brief Represents a request to perform an operation on a container.
/// @details Contains the operation type, runtime, and container name. Extend with additional fields as needed.
struct ContainerRequest {
    std::string operation;      ///< Operation type: "start", "stop", "create", etc.
    std::string runtime;        ///< Container runtime: "docker", "podman", etc.
    std::string container_name; ///< Name of the container to operate on.
    std::string image_name;     ///< Image name (used for create).
    std::string cpus;           ///< CPU limit (e.g., "0.5").
    std::string memory;         ///< Memory limit (e.g., "64").
    std::string pids;           ///< PIDs limit (e.g., "10").
    std::string restart_policy; ///< Restart policy (e.g., "unless-stopped")
    // Add more fields as needed
};