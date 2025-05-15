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

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& runtime_type, const std::string& command_type, const std::string& app_name) {
    if (runtime_type == RuntimeName::Docker) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<DockerCreateContainerCommand>(app_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<DockerStartContainerCommand>(app_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<DockerStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeName::DockerApi) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<DockerApiCreateContainerCommand>(app_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<DockerApiStartContainerCommand>(app_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<DockerApiStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeName::Podman) {
        if (command_type == CommandName::StartContainer) {
            return std::make_unique<PodmanStartContainerCommand>(app_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<PodmanStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeName::PodmanApi) {
        if (command_type == CommandName::CreateContainer) {
            return std::make_unique<PodmanApiCreateContainerCommand>(app_name);
        } else if (command_type == CommandName::StartContainer) {
            return std::make_unique<PodmanApiStartContainerCommand>(app_name);
        } else if (command_type == CommandName::StopContainer) {
            return std::make_unique<PodmanApiStopContainerCommand>(app_name);
        }
    } else {
        // Invalid runtime type
        CM_LOG_FATAL << "Invalid runtime type";
    }
    return nullptr;
}