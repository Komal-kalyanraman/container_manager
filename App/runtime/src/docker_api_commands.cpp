/**
 * @file docker_api_commands.cpp
 * @brief Implements Docker API command classes for managing containers via the Docker HTTP API over a Unix socket.
 *
 * This file provides implementations for commands such as checking Docker daemon availability,
 * creating, starting, stopping, restarting, and removing containers using the Docker REST API.
 * All HTTP requests are performed over the Unix socket using the CurlHandler utility.
 *
 * Usage:
 *   - Each command class encapsulates a specific Docker API operation.
 *   - The CurlHandler class is used to send HTTP requests over the Docker Unix socket.
 *   - Constants for the Docker socket path and API base URL should be defined in common.hpp.
 */

#include "inc/docker_api_commands.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

/**
 * @brief Command to check if the Docker daemon is available via the /_ping endpoint.
 */
DockerApiRuntimeAvailableCommand::DockerApiRuntimeAvailableCommand() {}

bool DockerApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "_ping";
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
 * @brief Command to create a new Docker container using the specified image and name.
 * @param container_name Name for the new container.
 * @param image_name Name of the image to use for the container.
 */
DockerApiCreateContainerCommand::DockerApiCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

bool DockerApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "containers/create?name=" + container_name_;
    std::string body = "{\"Image\": \"" + image_name_ + "\"}";

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
 * @brief Command to start a Docker container by name.
 * @param container_name Name of the container to start.
 */
DockerApiStartContainerCommand::DockerApiStartContainerCommand(const std::string& container_name) : container_name_(container_name) {}

bool DockerApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "containers/" + container_name_ + "/start";
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
 * @brief Command to stop a Docker container by name.
 * @param container_name Name of the container to stop.
 */
DockerApiStopContainerCommand::DockerApiStopContainerCommand(const std::string& container_name) : container_name_(container_name) {}

bool DockerApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "containers/" + container_name_ + "/stop";
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
 * @brief Command to restart a Docker container by name.
 * @param container_name Name of the container to restart.
 */
DockerApiRestartContainerCommand::DockerApiRestartContainerCommand(const std::string& container_name) : container_name_(container_name) {}

bool DockerApiRestartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "containers/" + container_name_ + "/restart";
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
 * @brief Command to remove a Docker container by name, using force removal.
 * @param container_name Name of the container to remove.
 */
DockerApiRemoveContainerCommand::DockerApiRemoveContainerCommand(const std::string& container_name) : container_name_(container_name) {}

bool DockerApiRemoveContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + "containers/" + container_name_ + "?force=true";
    bool success = curl.DeleteUnix(url, kDockerUnixSocketPath, response);
    if (success) {
        std::cout << "[Docker API] Remove container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] Failed to remove container. Response: " << response << std::endl;
        return false;
    }
}