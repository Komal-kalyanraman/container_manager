#pragma once

#include "cm_requestexecutor.hpp"

class JsonRequestExecutorHandler : public RequestExecutor {
public:
    nlohmann::json Execute(const std::string& data) override;
};