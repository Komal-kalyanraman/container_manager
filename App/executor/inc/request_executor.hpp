#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "inc/container_request.hpp"

class RequestExecutor {
public:
    virtual ~RequestExecutor() = default;
    // Accepts a string (could be JSON, Protobuf, etc.) and returns a JSON response
    virtual nlohmann::json Execute(const std::string& data) = 0;
};