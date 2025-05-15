#pragma once
#include <string>

struct ContainerRequest {
    std::string operation; // "start", "stop", "create", etc.
    std::string runtime;   // "docker", "podman", etc.
    std::string container_name;
    std::string image_name;
    std::string cpus;
    std::string memory;
    std::string pids;
    std::string restart_policy;
    // Add more fields as needed
};