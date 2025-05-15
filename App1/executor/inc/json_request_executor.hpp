#pragma once

#include "inc/request_executor.hpp"

class JsonRequestExecutorHandler : public RequestExecutor {
public:
    nlohmann::json Execute(const std::string& data) override;
};