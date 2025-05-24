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
            return std::make_unique<DockerCreateContainerCommand>(req);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<DockerStartContainerCommand>(req);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<DockerStopContainerCommand>(req);
        } else if (req.operation == CommandName::RemoveContainer) {
            return std::make_unique<DockerRemoveContainerCommand>(req);
        } else if (req.operation == CommandName::RestartContainer) {
            return std::make_unique<DockerRestartContainerCommand>(req);
        }
    } else if (req.runtime == RuntimeName::Podman) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<PodmanCreateContainerCommand>(req);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<PodmanStartContainerCommand>(req);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<PodmanStopContainerCommand>(req);
        } else if (req.operation == CommandName::RemoveContainer) {
            return std::make_unique<PodmanRemoveContainerCommand>(req);
        } else if (req.operation == CommandName::RestartContainer) {
            return std::make_unique<PodmanRestartContainerCommand>(req);
        }
    } else if (req.runtime == RuntimeName::DockerApi) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<DockerApiRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<DockerApiCreateContainerCommand>(req);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<DockerApiStartContainerCommand>(req);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<DockerApiStopContainerCommand>(req);
        } else if (req.operation == CommandName::RemoveContainer) {
            return std::make_unique<DockerApiRemoveContainerCommand>(req);
        } else if (req.operation == CommandName::RestartContainer) {
            return std::make_unique<DockerApiRestartContainerCommand>(req);
        }
    } else if (req.runtime == RuntimeName::PodmanApi) {
        if (req.operation == CommandName::RuntimeStatus) {
            return std::make_unique<PodmanApiRuntimeAvailableCommand>();
        } else if (req.operation == CommandName::CreateContainer) {
            return std::make_unique<PodmanApiCreateContainerCommand>(req);
        } else if (req.operation == CommandName::StartContainer) {
            return std::make_unique<PodmanApiStartContainerCommand>(req);
        } else if (req.operation == CommandName::StopContainer) {
            return std::make_unique<PodmanApiStopContainerCommand>(req);
        }
        // Add Remove/Restart if implemented for PodmanApi
    } else {
        CM_LOG_FATAL << "Invalid runtime type: " << req.runtime;
    }
    return nullptr;
}