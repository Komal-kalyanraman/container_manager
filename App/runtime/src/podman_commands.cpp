/**
 * @file podman_commands.cpp
 * @brief Implements Podman command classes for container operations.
 */

#include "inc/podman_commands.hpp"

#include <cstdlib>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/logging.hpp"

/**
 * @brief Constructs a PodmanRuntimeAvailableCommand.
 */
PodmanRuntimeAvailableCommand::PodmanRuntimeAvailableCommand() {
    // Empty constructor
}

/**
 * @brief Executes the command to check Podman runtime availability.
 * @return True if Podman is available, false otherwise.
 */
bool PodmanRuntimeAvailableCommand::Execute() const {
    int status = std::system("podman info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Podman is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Podman is not running";
        return false;
    }
}

/**
 * @brief Constructs a PodmanCreateContainerCommand.
 * @param container_name Name of the container to create.
 * @param image_name Name of the image to use for the container.
 */
PodmanCreateContainerCommand::PodmanCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

/**
 * @brief Executes the command to create a Podman container.
 * @return True if the container was created successfully, false otherwise.
 */
bool PodmanCreateContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Create,
        {{"runtime", "podman"}, {"name", container_name_}, {"image", image_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container created successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Podman container";
        return false;
    }
}

/**
 * @brief Constructs a PodmanStartContainerCommand.
 * @param container_name Name of the container to start.
 */
PodmanStartContainerCommand::PodmanStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

/**
 * @brief Executes the command to start a Podman container.
 * @return True if the container was started successfully, false otherwise.
 */
bool PodmanStartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Podman container";
        return false;
    }
}

/**
 * @brief Constructs a PodmanStopContainerCommand.
 * @param container_name Name of the container to stop.
 */
PodmanStopContainerCommand::PodmanStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

/**
 * @brief Executes the command to stop a Podman container.
 * @return True if the container was stopped successfully, false otherwise.
 */
bool PodmanStopContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Podman container";
        return false;
    }
}

/**
 * @brief Constructs a PodmanRemoveContainerCommand.
 * @param container_name Name of the container to remove.
 */
PodmanRemoveContainerCommand::PodmanRemoveContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

/**
 * @brief Executes the command to remove a Podman container.
 * @return True if the container was removed successfully, false otherwise.
 */
bool PodmanRemoveContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container removed successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to remove Podman container";
        return false;
    }
}

/**
 * @brief Constructs a PodmanRestartContainerCommand.
 * @param container_name Name of the container to restart.
 */
PodmanRestartContainerCommand::PodmanRestartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

/**
 * @brief Executes the command to restart a Podman container.
 * @return True if the container was restarted successfully, false otherwise.
 */
bool PodmanRestartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container restarted successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to restart Podman container";
        return false;
    }
}