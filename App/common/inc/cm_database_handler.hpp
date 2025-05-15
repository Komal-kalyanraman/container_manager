#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <cpp_redis/cpp_redis>

class DatabaseHandler {
public:
    static DatabaseHandler& GetInstance();

    // Save a JSON object to Redis with a given key
    void SaveJson(const std::string& key, const nlohmann::json& value);

    // (Optional) Retrieve a JSON object from Redis
    nlohmann::json GetJson(const std::string& key);

private:
    DatabaseHandler(); // private constructor
    ~DatabaseHandler();
    DatabaseHandler(const DatabaseHandler&) = delete;
    DatabaseHandler& operator=(const DatabaseHandler&) = delete;

    // Add your Redis connection object here, e.g.:
    cpp_redis::client redis_;
};