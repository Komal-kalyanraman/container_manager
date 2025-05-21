/**
 * @file podman_commands.cpp
 * @brief Implements Podman command classes for container operations.
 */

#include "inc/podman_commands.hpp"

#include <cstdlib>
#include <nlohmann/json.hpp>

#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/database_interface.hpp"
#include "inc/redis_database.hpp"

PodmanRuntimeAvailableCommand::PodmanRuntimeAvailableCommand() {
    // Empty constructor
}

bool PodmanRuntimeAvailableCommand::Execute() const {
    // Check if Podman is available
    int status = std::system("podman info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Podman is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Podman is not running";
        return false;
    }
}

PodmanCreateContainerCommand::PodmanCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

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

PodmanStartContainerCommand::PodmanStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanStartContainerCommand::Execute() const {
    // Start the Podman container
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container started successfully";
        db.UpdateField(container_name_, "status", "running");
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Podman container";
        db.UpdateField(container_name_, "status", "Error running container");
        return false;
    }
}

PodmanStopContainerCommand::PodmanStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanStopContainerCommand::Execute() const {
    // Stop the Podman container
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container stopped successfully";
        db.UpdateField(container_name_, "status", "stopped");
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Podman container";
        db.UpdateField(container_name_, "status", "Error stopping container");
        return false;
    }
}

PodmanRemoveContainerCommand::PodmanRemoveContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanRemoveContainerCommand::Execute() const {
    // Stop the Podman container
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container removed successfully";
        db.RemoveKey(container_name_);
        return true;
    } else {
        CM_LOG_ERROR << "Failed to remove Podman container";
        db.UpdateField(container_name_, "status", "Error removing container");
        return false;
    }
}

PodmanRestartContainerCommand::PodmanRestartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanRestartContainerCommand::Execute() const {
    // Stop the Podman container
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "podman"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container restarted successfully";
        db.UpdateField(container_name_, "status", "running");
        return true;
    } else {
        CM_LOG_ERROR << "Failed to restart Podman container";
        db.UpdateField(container_name_, "status", "Error restarting container");
        return false;
    }
}
