#include "inc/docker_commands.hpp"

#include <cstdlib>
#include <nlohmann/json.hpp>

#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/database_interface.hpp"
#include "inc/redis_database.hpp"

DockerRuntimeAvailableCommand::DockerRuntimeAvailableCommand() {
    // Empty constructor
}

bool DockerRuntimeAvailableCommand::Execute() const {
    // Check if Docker is available
    int status = std::system("docker info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Docker is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Docker is not running";
        return false;
    }
}

DockerCreateContainerCommand::DockerCreateContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerCreateContainerCommand::Execute() const {

    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    nlohmann::json config = db.GetJson(container_name_);

    std::string image_name_;
    if (!config["parameters"].empty() && config["parameters"][0].contains("image_name")) {
        image_name_ = config["parameters"][0]["image_name"].get<std::string>();
    } else {
        image_name_ = "";
    }
    // Start the Docker container
    std::string command = "docker create --name " + container_name_ + " " + image_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container created successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Docker container";
        return false;
    }
}

DockerStartContainerCommand::DockerStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerStartContainerCommand::Execute() const {
    // Start the Docker container
    std::string command = "docker start " + container_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Docker container";
        return false;
    }
}

DockerStopContainerCommand::DockerStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerStopContainerCommand::Execute() const {
    // Stop the Docker container
    std::string command = "docker stop " + container_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Docker container";
        return false;
    }
}
