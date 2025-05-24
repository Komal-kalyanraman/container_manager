/**
 * @file docker_api_commands.cpp
 * @brief Implements Docker API command classes for managing containers via the Docker HTTP API over a Unix socket.
 *
 * This file defines the logic for DockerApiRuntimeAvailableCommand, DockerApiCreateContainerCommand,
 * DockerApiStartContainerCommand, DockerApiStopContainerCommand, DockerApiRestartContainerCommand,
 * and DockerApiRemoveContainerCommand. Each command executes the corresponding Docker REST API operation
 * using parameters from the ContainerRequest structure and CurlHandler for HTTP communication.
 */

#include "inc/docker_api_commands.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

/**
 * @class DockerApiRuntimeAvailableCommand
 * @brief Command to check if Docker daemon is available via the API.
 */
DockerApiRuntimeAvailableCommand::DockerApiRuntimeAvailableCommand() {}

/// Executes the command to check Docker API runtime availability.
/// @return True if Docker is running, false otherwise.
bool DockerApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + std::string(ApiEndpoint::Ping);
    bool success = curl.GetUnix(url, kDockerUnixSocketPath, response);
    if (success && response == "OK") {
        std::cout << "[Docker API] Docker daemon is running (/_ping returned OK)." << std::endl;
        return true;
    } else if (success) {
        std::cerr << "[Docker API] Docker daemon responded, but not OK: " << response << std::endl;
        return false;
    } else {
        std::cerr << "[Docker API] Failed to contact Docker daemon." << std::endl;
        return false;
    }
}

/**
 * @class DockerApiCreateContainerCommand
 * @brief Command to create a Docker container via the Docker API.
 * @param req The container request containing all parameters for creation.
 */
DockerApiCreateContainerCommand::DockerApiCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Docker container via the API.
/// @return True if the container was created successfully, false otherwise.
bool DockerApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) +
                      std::string(ApiEndpoint::Create) +
                      req_.container_name;

    // Build JSON body with all relevant fields
    std::string body = "{";
    body += "\"Image\": \"" + req_.image_name + "\"";
    if (!req_.cpus.empty()) body += ", \"HostConfig\": {\"NanoCpus\": " + std::to_string(static_cast<long long>(std::stod(req_.cpus) * 1e9));
    if (!req_.memory.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"Memory\": " + std::to_string(std::stoll(req_.memory) * 1024 * 1024);
    }
    if (!req_.pids.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"PidsLimit\": " + req_.pids;
    }
    if (!req_.restart_policy.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"RestartPolicy\": {\"Name\": \"" + req_.restart_policy + "\"}";
    }
    if (body.find("HostConfig") != std::string::npos)
        body += "}";
    body += "}";

    bool success = curl.PostUnix(url, kDockerUnixSocketPath, body, response);
    if (success) {
        std::cout << "[Docker API] Create container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to create container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class DockerApiStartContainerCommand
 * @brief Command to start a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiStartContainerCommand::DockerApiStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Docker container via the API.
/// @return True if the container was started successfully, false otherwise.
bool DockerApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Start
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Docker API] Start container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to start container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class DockerApiStopContainerCommand
 * @brief Command to stop a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiStopContainerCommand::DockerApiStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Docker container via the API.
/// @return True if the container was stopped successfully, false otherwise.
bool DockerApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Stop
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Docker API] Stop container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to stop container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class DockerApiRestartContainerCommand
 * @brief Command to restart a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiRestartContainerCommand::DockerApiRestartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to restart a Docker container via the API.
/// @return True if the container was restarted successfully, false otherwise.
bool DockerApiRestartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Restart
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Docker API] Restart container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to restart container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class DockerApiRemoveContainerCommand
 * @brief Command to remove a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiRemoveContainerCommand::DockerApiRemoveContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to remove a Docker container via the API.
/// @return True if the container was removed successfully, false otherwise.
bool DockerApiRemoveContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Remove
    );
    bool success = curl.DeleteUnix(url, kDockerUnixSocketPath, response);
    if (success) {
        std::cout << "[Docker API] Remove container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to remove container. Response: " << response << std::endl;
        return false;
    }
}