#pragma once

#include <string>
#include <nlohmann/json.hpp>

class IDatabaseHandler {
public:
    virtual ~IDatabaseHandler() = default;
    virtual void SaveJson(const std::string& key, const nlohmann::json& value) = 0;
    virtual nlohmann::json GetJson(const std::string& key) = 0;
    virtual void ClearDatabase() = 0;
};