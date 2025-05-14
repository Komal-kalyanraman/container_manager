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

const int kMaxThreadPoolSize = 4;

const int kHttpServerPort = 5000;
const std::string kHttpServerHost = "0.0.0.0";

const std::string kDockerRuntime = "docker";
const std::string kDockerApiRuntime = "docker-api";
const std::string kPodmanRuntime = "podman";
const std::string kPodmanApiRuntime = "podman-api";

const std::string kRuntimeAvailable = "available";
const std::string kRuntimeStartContainer = "start";
const std::string kRuntimeStopContainer = "stop";
const std::string kRuntimeCreateContainer = "create";
const std::string kRuntimeRestartContainer = "restart";
const std::string kRuntimeRemoveContainer = "remove";
