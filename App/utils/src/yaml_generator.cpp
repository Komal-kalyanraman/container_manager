#include "inc/yaml_generator.hpp"
#include "inc/common.hpp"
#include <fstream>

std::string GenerateKubernetesPodYaml(const ContainerRequest& req, bool include_security_context) {
    std::string yaml_file_path = std::string(kPodmanYamlStoragePath) + req.container_name + kPodFileSuffix;
    std::ofstream yaml_file(yaml_file_path);

    if (!yaml_file.is_open()) {
        throw std::runtime_error("Failed to create YAML file: " + yaml_file_path);
    }

    yaml_file << "apiVersion: v1\n";
    yaml_file << "kind: Pod\n";
    yaml_file << "metadata:\n";
    yaml_file << "  name: " << req.container_name << kPodNameSuffix << "\n";
    yaml_file << "spec:\n";
    yaml_file << "  containers:\n";
    yaml_file << "    - name: " << req.container_name << "\n";
    yaml_file << "      image: " << req.image_name << "\n";
    yaml_file << "      resources:\n";
    yaml_file << "        limits:\n";
    yaml_file << "          cpu: \"" << req.cpus << "\"\n";
    yaml_file << "          memory: \"" << req.memory << "Mi\"\n";
    
    // Include securityContext only if requested (for Podman compatibility)
    if (include_security_context) {
        yaml_file << "      securityContext:\n";
        yaml_file << "        pidsLimit: " << req.pids << "\n";
        yaml_file << "      restartPolicy: \"" << req.restart_policy << "\"\n";
    }
    
    yaml_file << "      ports:\n";
    yaml_file << "        - containerPort: 80\n";
    yaml_file << "          hostPort: 8080\n";
    
    // Add restartPolicy at pod level for Kubernetes compatibility
    if (!include_security_context) {
        yaml_file << "  restartPolicy: Always\n";
    }

    yaml_file.close();
    return yaml_file_path;
}