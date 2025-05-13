#pragma once

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
