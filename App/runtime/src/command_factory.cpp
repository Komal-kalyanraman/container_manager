#include "inc/command_factory.hpp"

#include <iostream>

#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/podman_commands.hpp"
#include "inc/docker_commands.hpp"
#include "inc/podman_api_commands.hpp"
#include "inc/docker_api_commands.hpp"

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& runtime_type, const std::string& command_type) {
    if (runtime_type == RuntimeName::Docker) {
        if (command_type == CommandName::RuntimeStatus) {
            return std::make_unique<DockerRuntimeAvailableCommand>();
        }
    } else if (runtime_type == RuntimeName::DockerApi) {
        if (command_type == CommandName::RuntimeStatus) {
            return std::make_unique<DockerApiRuntimeAvailableCommand>();
        }
    } else if (runtime_type == RuntimeName::Podman) {
        if (command_type == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanRuntimeAvailableCommand>();
        }
    } else if (runtime_type == RuntimeName::PodmanApi) {
        if (command_type == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanApiRuntimeAvailableCommand>();
        }
    } else {
        // Invalid runtime type
        CM_LOG_FATAL << "Invalid runtime type";
    }
    return nullptr;
}

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& runtime_type, const std::string& command_type, const std::string& container_name) {
    if (runtime_type == RuntimeName::Docker) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<DockerCreateContainerCommand>(container_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<DockerStartContainerCommand>(container_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<DockerStopContainerCommand>(container_name);
        } else if (command_type == CommandName::RemoveContainer) {
            return std::make_unique<DockerRemoveContainerCommand>(container_name);
        } else if (command_type == CommandName::RestartContainer) {
            return std::make_unique<DockerRestartContainerCommand>(container_name);
        }
    } else if (runtime_type == RuntimeName::DockerApi) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<DockerApiCreateContainerCommand>(container_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<DockerApiStartContainerCommand>(container_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<DockerApiStopContainerCommand>(container_name);
        }
    } else if (runtime_type == RuntimeName::Podman) {
        if (command_type == CommandName::StartContainer) {
            return std::make_unique<PodmanStartContainerCommand>(container_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<PodmanStopContainerCommand>(container_name);
        }
    } else if (runtime_type == RuntimeName::PodmanApi) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<PodmanApiCreateContainerCommand>(container_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<PodmanApiStartContainerCommand>(container_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<PodmanApiStopContainerCommand>(container_name);
        }
    } else {
        // Invalid runtime type
        CM_LOG_FATAL << "Invalid runtime type";
    }
    return nullptr;
}