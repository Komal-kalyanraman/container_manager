#include "inc/cm_container_service_handler.hpp"

#include <iostream>

#include <nlohmann/json.hpp>

nlohmann::json ContainerServiceHandler::HandleRequest(const ContainerRequest& req) {
    // Example: Just echo the request for now
    std::cout << "Handling operation: " << req.operation << " for container: " << req.container_name << std::endl;
    // TODO: Integrate with your command pattern and actual logic
    return {{"status", "success"}, {"operation", req.operation}, {"container", req.container_name}};
}