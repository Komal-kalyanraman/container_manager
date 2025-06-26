/**
 * @file docker_commands.cpp
 * @brief Implements Docker command classes for container operations.
 *
 * This file defines the logic for DockerRuntimeAvailableCommand, DockerCreateContainerCommand,
 * DockerStartContainerCommand, DockerStopContainerCommand, DockerRemoveContainerCommand,
 * and DockerRestartContainerCommand. Each command executes the corresponding Docker CLI operation
 * using parameters from the ContainerRequest structure and provides standardized error handling and logging.
 */

#include "inc/docker_commands.hpp"
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/logging.hpp"

/**
 * @class DockerRuntimeAvailableCommand
 * @brief Command to check if Docker runtime is available.
 */
DockerRuntimeAvailableCommand::DockerRuntimeAvailableCommand() {}

/// Executes the command to check Docker runtime availability.
/// @return Status indicating if Docker is running.
Status DockerRuntimeAvailableCommand::Execute() const {
    int status = std::system("docker info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Docker is running " << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Docker runtime is not running, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerCreateContainerCommand
 * @brief Command to create a Docker container with user-defined parameters.
 * @param req The container request containing all parameters for creation.
 */
DockerCreateContainerCommand::DockerCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Docker container.
/// @return Status indicating if the container was created successfully.
Status DockerCreateContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Create,
        {
            {"runtime", "docker"},
            {"name", req_.container_name},
            {"image", req_.image_name},
            {"cpus", req_.cpus},
            {"memory", req_.memory},
            {"pids", req_.pids},
            {"restart_policy", req_.restart_policy}
        }
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container created successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to create Docker container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerStartContainerCommand
 * @brief Command to start a Docker container.
 * @param req The container request containing the container name.
 */
DockerStartContainerCommand::DockerStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Docker container.
/// @return Status indicating if the container was started successfully.
Status DockerStartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container started successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to start Docker container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerStopContainerCommand
 * @brief Command to stop a Docker container.
 * @param req The container request containing the container name.
 */
DockerStopContainerCommand::DockerStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Docker container.
/// @return Status indicating if the container was stopped successfully.
Status DockerStopContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container stopped successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to stop Docker container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerRemoveContainerCommand
 * @brief Command to remove a Docker container.
 * @param req The container request containing the container name.
 */
DockerRemoveContainerCommand::DockerRemoveContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to remove a Docker container.
/// @return Status indicating if the container was removed successfully.
Status DockerRemoveContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container removed successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to remove Docker container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerRestartContainerCommand
 * @brief Command to restart a Docker container.
 * @param req The container request containing the container name.
 */
DockerRestartContainerCommand::DockerRestartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to restart a Docker container.
/// @return Status indicating if the container was restarted successfully.
Status DockerRestartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container restarted successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to restart Docker container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}