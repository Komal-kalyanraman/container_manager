#pragma once

#include <string>

/// @brief Available container runtime types
enum class RuntimeType { 
    Docker, 
    Podman, 
    DockerApi, 
    PodmanApi 
};

/// @brief Available command types
enum class CommandType {
    RuntimeStatus,
    StartContainer,
    StopContainer,
    CreateContainer,
};

const int kHttpServerPort = 5000;
const std::string kHttpServerHost = "0.0.0.0";