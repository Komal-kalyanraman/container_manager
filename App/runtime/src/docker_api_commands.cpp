#include "inc/docker_api_commands.hpp"

#include "inc/common.hpp"
#include "inc/logging.hpp"

DockerApiRuntimeAvailableCommand::DockerApiRuntimeAvailableCommand() {
    // Empty constructor
}
bool DockerApiRuntimeAvailableCommand::Execute() const {
    return true;
}

DockerApiCreateContainerCommand::DockerApiCreateContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerApiCreateContainerCommand::Execute() const {
    return true;
}

DockerApiStartContainerCommand::DockerApiStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerApiStartContainerCommand::Execute() const {
    return true;
}

DockerApiStopContainerCommand::DockerApiStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerApiStopContainerCommand::Execute() const {
    return true;
}
