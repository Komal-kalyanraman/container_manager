#include "inc/cm_command_factory.hpp"

#include <iostream>

#include "inc/cm_common.hpp"
#include "inc/cm_logging.hpp"
#include "inc/cm_podman_commands.hpp"
#include "inc/cm_docker_commands.hpp"
#include "inc/cm_podman_api_commands.hpp"
#include "inc/cm_docker_api_commands.hpp"

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& runtime_type, const std::string& command_type) {
    if (runtime_type == "Docker") {
        if (command_type == "Available") {
            return std::make_unique<DockerRuntimeAvailableCommand>();
        }
    } else if (runtime_type == "DockerApi") {
        if (command_type == "Available") {
            return std::make_unique<DockerApiRuntimeAvailableCommand>();
        }
    } else if (runtime_type == "Podman") {
        if (command_type == "Available") {
            return std::make_unique<PodmanRuntimeAvailableCommand>();
        }
    } else if (runtime_type == "PodmanApi") {
        if (command_type == "Available") {
            return std::make_unique<PodmanApiRuntimeAvailableCommand>();
        }
    } else {
        // Invalid runtime type
        CM_LOG_FATAL << "Invalid runtime type";
    }
    return nullptr;
}

std::unique_ptr<Command> CommandFactory::CreateCommand(const std::string& runtime_type, const std::string& command_type, const std::string& app_name) {
    if (runtime_type == "Docker") {
        if (command_type == "StartContainer") {
            return std::make_unique<DockerStartContainerCommand>(app_name);
        } else if (command_type == "StopContainer") {
            return std::make_unique<DockerStopContainerCommand>(app_name);
        }
    } else if (runtime_type == "DockerApi") {
        if (command_type == "CreateContainer") {
            return std::make_unique<DockerApiCreateContainerCommand>(app_name);
        } else if (command_type == "StartContainer") {
            return std::make_unique<DockerApiStartContainerCommand>(app_name);
        } else if (command_type == "StopContainer") {
            return std::make_unique<DockerApiStopContainerCommand>(app_name);
        }
    } else if (runtime_type == "Podman") {
        if (command_type == "StartContainer") {
            return std::make_unique<PodmanStartContainerCommand>(app_name);
        } else if (command_type == "StopContainer") {
            return std::make_unique<PodmanStopContainerCommand>(app_name);
        }
    } else if (runtime_type == "PodmanApi") {
        if (command_type == "CreateContainer") {
            return std::make_unique<PodmanApiCreateContainerCommand>(app_name);
        } else if (command_type == "StartContainer") {
            return std::make_unique<PodmanApiStartContainerCommand>(app_name);
        } else if (command_type == "StopContainer") {
            return std::make_unique<PodmanApiStopContainerCommand>(app_name);
        }
    } else {
        // Invalid runtime type
        CM_LOG_FATAL << "Invalid runtime type";
    }
    return nullptr;
}