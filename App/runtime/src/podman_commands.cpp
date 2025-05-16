/**
 * @file podman_commands.hpp
 * @brief Declares Podman command classes for container operations.
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

PodmanCreateContainerCommand::PodmanCreateContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanCreateContainerCommand::Execute() const {

    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    nlohmann::json config = db.GetJson(container_name_);

    std::string image_name_;
    if (!config["parameters"].empty() && config["parameters"][0].contains("image_name")) {
        image_name_ = config["parameters"][0]["image_name"].get<std::string>();
    } else {
        image_name_ = "";
    }
    // Start the Podman container
    std::string command = "podman create --name " + container_name_ + " " + image_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container created successfully";
        db.UpdateField(container_name_, "status", "created");
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Podman container";
        db.UpdateField(container_name_, "status", "Error creating container");
        return false;
    }
}

PodmanStartContainerCommand::PodmanStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanStartContainerCommand::Execute() const {
    // Start the Podman container
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    std::string command = "podman start " + container_name_;
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
    std::string command = "podman stop " + container_name_;
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
    std::string command = "podman rm -f " + container_name_;
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
    std::string command = "podman restart " + container_name_;
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
