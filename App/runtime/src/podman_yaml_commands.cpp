#include "inc/podman_yaml_commands.hpp"
#include <fstream>
#include <cstdlib>
#include "inc/logging.hpp"

/**
 * @class PodmanYamlPlayCommand
 * @brief Command to play a Podman YAML file (create resources).
 */
PodmanYamlPlayCommand::PodmanYamlPlayCommand(const ContainerRequest& req)
    : req_(req) {}

/// Generates a YAML file based on the container request.
/// @return Path to the generated YAML file.
std::string PodmanYamlPlayCommand::GenerateYamlFile() const {
    std::string yaml_file_path = "../../storage/podman_yaml/" + req_.container_name + "_pod.yaml";
    std::ofstream yaml_file(yaml_file_path);

    yaml_file << "apiVersion: v1\n";
    yaml_file << "kind: Pod\n";
    yaml_file << "metadata:\n";
    yaml_file << "  name: " << req_.container_name << "_pod\n";  // Changed: Added "_pod" suffix
    yaml_file << "spec:\n";
    yaml_file << "  containers:\n";
    yaml_file << "    - name: " << req_.container_name << "\n";  // Container name remains as original
    yaml_file << "      image: " << req_.image_name << "\n";
    yaml_file << "      resources:\n";
    yaml_file << "        limits:\n";
    yaml_file << "          cpu: \"" << req_.cpus << "\"\n";
    yaml_file << "          memory: \"" << req_.memory << "Mi\"\n";
    yaml_file << "      securityContext:\n";
    yaml_file << "        pidsLimit: " << req_.pids << "\n";
    yaml_file << "      restartPolicy: \"" << req_.restart_policy << "\"\n";
    yaml_file << "      ports:\n";
    yaml_file << "        - containerPort: 80\n";
    yaml_file << "          hostPort: 8080\n";

    yaml_file.close();
    return yaml_file_path;
}

/// Executes the command to play the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlPlayCommand::Execute() const {
    std::string yaml_file_path = GenerateYamlFile();
    std::string command = "podman play kube " + yaml_file_path;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file played successfully: " << yaml_file_path << std::endl;
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
PodmanYamlDeleteCommand::PodmanYamlDeleteCommand(const ContainerRequest& req)
    : req_(req) {}

/// Generates a YAML file based on the container request.
/// @return Path to the generated YAML file.
std::string PodmanYamlDeleteCommand::GenerateYamlFile() const {
    std::string yaml_file_path = "../../storage/podman_yaml/" + req_.container_name + "_pod.yaml";
    std::ofstream yaml_file(yaml_file_path);

    yaml_file << "apiVersion: v1\n";
    yaml_file << "kind: Pod\n";
    yaml_file << "metadata:\n";
    yaml_file << "  name: " << req_.container_name << "_pod\n";  // Changed: Added "_pod" suffix
    yaml_file << "spec:\n";
    yaml_file << "  containers:\n";
    yaml_file << "    - name: " << req_.container_name << "\n";  // Container name remains as original
    yaml_file << "      image: " << req_.image_name << "\n";
    yaml_file << "      resources:\n";
    yaml_file << "        limits:\n";
    yaml_file << "          cpu: \"" << req_.cpus << "\"\n";
    yaml_file << "          memory: \"" << req_.memory << "Mi\"\n";
    yaml_file << "      securityContext:\n";
    yaml_file << "        pidsLimit: " << req_.pids << "\n";
    yaml_file << "      restartPolicy: \"" << req_.restart_policy << "\"\n";
    yaml_file << "      ports:\n";
    yaml_file << "        - containerPort: 80\n";
    yaml_file << "          hostPort: 8080\n";

    yaml_file.close();
    return yaml_file_path;
}

/// Executes the command to delete resources defined in the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlDeleteCommand::Execute() const {
    std::string yaml_file_path = GenerateYamlFile();
    std::string command = "podman play kube --down " + yaml_file_path;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file resources deleted successfully: " << yaml_file_path << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to delete Podman YAML file resources, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}