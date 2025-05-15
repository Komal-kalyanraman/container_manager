#pragma once
#include <string>

struct ContainerRequest {
    std::string operation; // "start", "stop", "create", etc.
    std::string runtime;   // "docker", "podman", etc.
    std::string container_name;
    // Add more fields as needed
};