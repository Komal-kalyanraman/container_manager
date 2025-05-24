#include "inc/docker_commands.hpp"
#include <cstdlib>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/logging.hpp"

DockerRuntimeAvailableCommand::DockerRuntimeAvailableCommand() {}

bool DockerRuntimeAvailableCommand::Execute() const {
    int status = std::system("docker info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Docker is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Docker is not running";
        return false;
    }
}

DockerCreateContainerCommand::DockerCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

bool DockerCreateContainerCommand::Execute() const {
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
        CM_LOG_INFO << "Docker container created successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Docker container";
        return false;
    }
}

DockerStartContainerCommand::DockerStartContainerCommand(const ContainerRequest& req) : req_(req) {}

bool DockerStartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Docker container";
        return false;
    }
}

DockerStopContainerCommand::DockerStopContainerCommand(const ContainerRequest& req) : req_(req) {}

bool DockerStopContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Docker container";
        return false;
    }
}

DockerRemoveContainerCommand::DockerRemoveContainerCommand(const ContainerRequest& req) : req_(req) {}

bool DockerRemoveContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container removed successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to remove Docker container";
        return false;
    }
}

DockerRestartContainerCommand::DockerRestartContainerCommand(const ContainerRequest& req) : req_(req) {}

bool DockerRestartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "docker"}, {"name", req_.container_name}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container restarted successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to restart Docker container";
        return false;
    }
}