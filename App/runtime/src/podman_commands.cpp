/**
 * @file podman_commands.cpp
 * @brief Implements Podman command classes for container operations.
 *
 * This file defines the logic for PodmanRuntimeAvailableCommand, PodmanCreateContainerCommand,
 * PodmanStartContainerCommand, PodmanStopContainerCommand, PodmanRemoveContainerCommand,
 * and PodmanRestartContainerCommand. Each command executes the corresponding Podman CLI operation
 * using parameters from the ContainerRequest structure.
 */

#include "inc/podman_commands.hpp"
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/logging.hpp"

/**
 * @class PodmanRuntimeAvailableCommand
 * @brief Command to check if Podman runtime is available.
 */
PodmanRuntimeAvailableCommand::PodmanRuntimeAvailableCommand() {}

/// Executes the command to check Podman runtime availability.
/// @return Status indicating if Podman is running.
Status PodmanRuntimeAvailableCommand::Execute() const {
    int status = std::system("podman info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Podman is running " << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Podman runtime is not running, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanCreateContainerCommand
 * @brief Command to create a Podman container with user-defined parameters.
 * @param req The container request containing all parameters for creation.
 */
PodmanCreateContainerCommand::PodmanCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Podman container.
/// @return Status indicating if the container was created successfully.
Status PodmanCreateContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Create,
        {
            {"runtime", "podman"},
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
        CM_LOG_INFO << "Podman container created successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to create Podman container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanStartContainerCommand
 * @brief Command to start a Podman container.
 * @param req The container request containing the container name.
 */
PodmanStartContainerCommand::PodmanStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Podman container.
/// @return Status indicating if the container was started successfully.
Status PodmanStartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "podman"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container started successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to start Podman container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanStopContainerCommand
 * @brief Command to stop a Podman container.
 * @param req The container request containing the container name.
 */
PodmanStopContainerCommand::PodmanStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Podman container.
/// @return Status indicating if the container was stopped successfully.
Status PodmanStopContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "podman"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container stopped successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to stop Podman container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanRemoveContainerCommand
 * @brief Command to remove a Podman container.
 * @param req The container request containing the container name.
 */
PodmanRemoveContainerCommand::PodmanRemoveContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to remove a Podman container.
/// @return Status indicating if the container was removed successfully.
Status PodmanRemoveContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "podman"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container removed successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to remove Podman container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanRestartContainerCommand
 * @brief Command to restart a Podman container.
 * @param req The container request containing the container name.
 */
PodmanRestartContainerCommand::PodmanRestartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to restart a Podman container.
/// @return Status indicating if the container was restarted successfully.
Status PodmanRestartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "podman"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container restarted successfully" << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to restart Podman container, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}