#include "inc/docker_api_commands.hpp"

#include "inc/common.hpp"
#include "inc/logging.hpp"

#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

DockerApiRuntimeAvailableCommand::DockerApiRuntimeAvailableCommand() {}

bool DockerApiRuntimeAvailableCommand::Execute() const {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "[Docker API] Failed to initialize CURL." << std::endl;
        return false;
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, kDockerUnixSocketPath);
    curl_easy_setopt(curl, CURLOPT_URL, (std::string(kDockerApiBaseUrl) + "images/json").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
        size_t totalSize = size * nmemb;
        std::string* str = static_cast<std::string*>(userp);
        str->append(static_cast<char*>(contents), totalSize);
        return totalSize;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res == CURLE_OK) {
        std::cout << "[Docker API] Images JSON:\n" << response << std::endl;
        return true;
    } else {
        std::cerr << "[Docker API] CURL error: " << curl_easy_strerror(res) << std::endl;
        return false;
    }
    // return true;
}

DockerApiCreateContainerCommand::DockerApiCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {
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
