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
    std::string operation; // "start", "stop", "create", etc.
    std::string runtime;   // "docker", "podman", etc.
    std::string container_name;
    std::string image_name;      // Image name (used for create)
    // Add more fields as needed
};