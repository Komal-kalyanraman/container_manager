#pragma once
#include "inc/database_interface.hpp"
#include <cpp_redis/cpp_redis>
#include <string>
#include <nlohmann/json.hpp>

class RedisDatabaseHandler : public IDatabaseHandler {
public:
    static RedisDatabaseHandler& GetInstance();

    void SaveJson(const std::string& key, const nlohmann::json& value) override;
    nlohmann::json GetJson(const std::string& key) override;
    void ClearDatabase() override;

private:
    RedisDatabaseHandler();
    ~RedisDatabaseHandler();
    RedisDatabaseHandler(const RedisDatabaseHandler&) = delete;
    RedisDatabaseHandler& operator=(const RedisDatabaseHandler&) = delete;

    cpp_redis::client redis_;
};