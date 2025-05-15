#pragma once

#include <nlohmann/json.hpp>
#include "inc/cm_container_request.hpp"

class ContainerServiceHandler {
public:
    static nlohmann::json HandleRequest(const ContainerRequest& req);
    static bool CheckRuntimeAvailable(const std::string& runtime, const std::string& operation);
    static bool CheckCreateContainer(const std::string& runtime, const std::string& operation, const std::string& app_name);
};