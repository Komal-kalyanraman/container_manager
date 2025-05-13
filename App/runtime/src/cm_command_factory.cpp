
#include "inc/cm_command_factory.hpp"

#include <iostream>
#include <glog/logging.h>

#include "inc/cm_common.hpp"
#include "inc/cm_podman_commands.hpp"
#include "inc/cm_docker_commands.hpp"
#include "inc/cm_podman_api_commands.hpp"
#include "inc/cm_docker_api_commands.hpp"

std::unique_ptr<Command> CommandFactory::CreateCommand(RuntimeType runtime_type, CommandType command_type) {
    if (runtime_type == RuntimeType::Docker) {
        if (command_type == CommandType::RuntimeStatus) {
            return std::make_unique<DockerRuntimeAvailableCommand>();
            // LOG(INFO) << "Docker runtime available";
        }
    } else if (runtime_type == RuntimeType::DockerApi) {
        if (command_type == CommandType::RuntimeStatus) {
            return std::make_unique<DockerApiRuntimeAvailableCommand>();
        }
    } else if (runtime_type == RuntimeType::Podman) {
        if (command_type == CommandType::RuntimeStatus) {
            return std::make_unique<PodmanRuntimeAvailableCommand>();
        }
    } else if (runtime_type == RuntimeType::PodmanApi) {
        if (command_type == CommandType::RuntimeStatus) {
            return std::make_unique<PodmanApiRuntimeAvailableCommand>();
        }
    } else {
        // Invalid runtime type
        std::cerr << "Invalid runtime type" << std::endl;
        LOG(FATAL) << "Invalid runtime type";
    }
    return nullptr;
}

std::unique_ptr<Command> CommandFactory::CreateCommand(RuntimeType runtime_type, CommandType command_type, const std::string& app_name) {
    if (runtime_type == RuntimeType::Docker) {
        if (command_type == CommandType::StartContainer) {
            return std::make_unique<DockerStartContainerCommand>(app_name);
        } else if (command_type == CommandType::StopContainer) {
            return std::make_unique<DockerStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeType::DockerApi) {
        if (command_type == CommandType::CreateContainer) {
            return std::make_unique<DockerApiCreateContainerCommand>(app_name);
        } else if (command_type == CommandType::StartContainer) {
            return std::make_unique<DockerApiStartContainerCommand>(app_name);
        } else if (command_type == CommandType::StopContainer) {
            return std::make_unique<DockerApiStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeType::Podman) {
        if (command_type == CommandType::StartContainer) {
            return std::make_unique<PodmanStartContainerCommand>(app_name);
        } else if (command_type == CommandType::StopContainer) {
            return std::make_unique<PodmanStopContainerCommand>(app_name);
        }
    } else if (runtime_type == RuntimeType::PodmanApi) {
        if (command_type == CommandType::CreateContainer) {
            return std::make_unique<PodmanApiCreateContainerCommand>(app_name);
        } else if (command_type == CommandType::StartContainer) {
            return std::make_unique<PodmanApiStartContainerCommand>(app_name);
        } else if (command_type == CommandType::StopContainer) {
            return std::make_unique<PodmanApiStopContainerCommand>(app_name);
        }
    } else {
        // Invalid runtime type
        std::cerr << "Invalid runtime type" << std::endl;
        LOG(FATAL) << "Invalid runtime type";
    }
    return nullptr;

}