/**
 * @file podman_api_commands.cpp
 * @brief Implements Podman API command classes for managing containers via the Podman HTTP API over a Unix socket.
 *
 * This file provides implementations for commands such as checking Podman daemon availability,
 * creating, starting, and stopping containers using the Podman REST API.
 * All HTTP requests are performed over the Unix socket using the CurlHandler utility.
 *
 * Usage:
 *   - Each command class encapsulates a specific Podman API operation.
 *   - The CurlHandler class is used to send HTTP requests over the Podman Unix socket.
 *   - Constants for the Podman socket path and API base URL should be defined in common.hpp or locally.
 */

#include "inc/podman_api_commands.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

/**
 * @brief Command to check if the Podman daemon is available via the /_ping endpoint.
 */
PodmanApiRuntimeAvailableCommand::PodmanApiRuntimeAvailableCommand() {}

bool PodmanApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + "_ping";
    bool success = curl.GetUnix(url, kPodmanUnixSocketPath, response);
    if (success && response == "OK") {
        std::cout << "[Podman API] Podman daemon is running (/_ping returned OK)." << std::endl;
        return true;
    } else if (success) {
        std::cerr << "[Podman API] Podman daemon responded, but not OK: " << response << std::endl;
        return false;
    } else {
        std::cerr << "[Podman API] Failed to contact Podman daemon." << std::endl;
        return false;
    }
}

/**
 * @brief Command to create a new Podman container using the specified image and name.
 * @param container_name Name for the new container.
 * @param image_name Name of the image to use for the container.
 */
PodmanApiCreateContainerCommand::PodmanApiCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

bool PodmanApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + "containers/create?name=" + container_name_;
    std::string body = "{\"Image\": \"" + image_name_ + "\"}";
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, body, response);
    if (success) {
        std::cout << "[Podman API] Create container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to create container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @brief Command to start a Podman container by name.
 * @param container_name Name of the container to start.
 */
PodmanApiStartContainerCommand::PodmanApiStartContainerCommand(const std::string& container_name)
 : container_name_(container_name) {}

bool PodmanApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + "containers/" + container_name_ + "/start";
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Podman API] Start container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to start container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @brief Command to stop a Podman container by name.
 * @param container_name Name of the container to stop.
 */
PodmanApiStopContainerCommand::PodmanApiStopContainerCommand(const std::string& container_name) 
: container_name_(container_name) {}

bool PodmanApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + "containers/" + container_name_ + "/stop";
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Podman API] Stop container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to stop container. Response: " << response << std::endl;
        return false;
    }
}