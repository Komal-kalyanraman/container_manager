#include "inc/podman_yaml_commands.hpp"
#include <cstdlib>
#include "inc/logging.hpp"

/**
 * @class PodmanYamlPlayCommand
 * @brief Command to play a Podman YAML file (create resources).
 */
PodmanYamlPlayCommand::PodmanYamlPlayCommand(const std::string& yaml_file_path)
    : yaml_file_path_(yaml_file_path) {}

/// Executes the command to play the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlPlayCommand::Execute() const {
    std::string command = "podman play kube " + yaml_file_path_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file played successfully: " << yaml_file_path_ << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to play Podman YAML file, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanYamlDeleteCommand
 * @brief Command to delete resources defined in a Podman YAML file.
 */
PodmanYamlDeleteCommand::PodmanYamlDeleteCommand(const std::string& yaml_file_path)
    : yaml_file_path_(yaml_file_path) {}

/// Executes the command to delete resources defined in the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlDeleteCommand::Execute() const {
    std::string command = "podman play kube --down " + yaml_file_path_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file resources deleted successfully: " << yaml_file_path_ << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to delete Podman YAML file resources, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}