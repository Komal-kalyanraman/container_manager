/**
 * @file command_factory.cpp
 * @brief Implements the CommandFactory class for creating command objects based on container requests.
 */

#include "inc/command_factory.hpp"

#include <iostream>
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/podman_commands.hpp"
#include "inc/docker_commands.hpp"
#include "inc/podman_api_commands.hpp"
#include "inc/docker_api_commands.hpp"
#include "inc/container_request.hpp"

/**
 * @brief Factory method to create a command object based on the container request.
 * @param req The container request containing runtime, operation, and other details.
 * @return A unique pointer to the appropriate Command object, or nullptr if invalid.
 */
std::unique_ptr<Command> CommandFactory::CreateCommand(const ContainerRequest& req) {
    if (req.runtime == RuntimeName::Docker) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<DockerRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<DockerCreateContainerCommand>(req.container_name, req.image_name);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<DockerStartContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<DockerStopContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::RemoveContainer) {
            return std::make_unique<DockerRemoveContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::RestartContainer) {
            return std::make_unique<DockerRestartContainerCommand>(req.container_name);
        }
    } else if (req.runtime == RuntimeName::Podman) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<PodmanCreateContainerCommand>(req.container_name, req.image_name);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<PodmanStartContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<PodmanStopContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::RemoveContainer) {
            return std::make_unique<PodmanRemoveContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::RestartContainer) {
            return std::make_unique<PodmanRestartContainerCommand>(req.container_name);
        }
    } else if (req.runtime == RuntimeName::DockerApi) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<DockerApiRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<DockerApiCreateContainerCommand>(req.container_name, req.image_name);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<DockerApiStartContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<DockerApiStopContainerCommand>(req.container_name);
        }
        // Add Remove/Restart if implemented for DockerApi
    } else if (req.runtime == RuntimeName::PodmanApi) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanApiRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<PodmanApiCreateContainerCommand>(req.container_name, req.image_name);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<PodmanApiStartContainerCommand>(req.container_name);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<PodmanApiStopContainerCommand>(req.container_name);
        }
        // Add Remove/Restart if implemented for PodmanApi
    } else {
        CM_LOG_FATAL << "Invalid runtime type: " << req.runtime;
    }
    return nullptr;
}