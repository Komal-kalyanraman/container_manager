#include "inc/redis_database.hpp"

#include <iostream>

RedisDatabaseHandler::RedisDatabaseHandler() {
    // Connect to Redis server (adjust host/port as needed)
    redis_.connect();
}

RedisDatabaseHandler::~RedisDatabaseHandler() {
    redis_.disconnect();
}

RedisDatabaseHandler& RedisDatabaseHandler::GetInstance() {
    static RedisDatabaseHandler instance;
    return instance;
}

void RedisDatabaseHandler::SaveJson(const std::string& key, const nlohmann::json& value) {
    // Save JSON as string
    std::string json_str = value.dump();
    redis_.set(key, json_str);
    redis_.sync_commit();
    std::cout << "[RedisDatabaseHandler] Saving to Redis: key=" << key << ", value=" << json_str << std::endl;
}

nlohmann::json RedisDatabaseHandler::GetJson(const std::string& key) {
    try {
        auto reply = redis_.get(key);
        redis_.sync_commit();
        auto result = reply.get().as_string();
        if (result.empty()) {
            std::cerr << "[RedisDatabaseHandler] Key not found: " << key << std::endl;
            return nlohmann::json{}; // Return empty JSON object
        }
        return nlohmann::json::parse(result);
    } catch (const std::exception& e) {
        std::cerr << "[RedisDatabaseHandler] Error reading key " << key << ": " << e.what() << std::endl;
        return nlohmann::json{};
    }
}

void RedisDatabaseHandler::ClearDatabase() {
    redis_.flushdb();
    redis_.sync_commit();
}

void RedisDatabaseHandler::UpdateField(const std::string& key, const std::string& field, const std::string& value) {
    nlohmann::json obj = GetJson(key);
    if (obj.is_null()) {
        std::cerr << "[RedisDatabaseHandler] Cannot update field, key not found: " << key << std::endl;
        return;
    }
    obj[field] = value;
    SaveJson(key, obj);
}

void RedisDatabaseHandler::RemoveKey(const std::string& key) {
    redis_.del({key});
    redis_.sync_commit();
    std::cout << "[RedisDatabaseHandler] Removed key from Redis: " << key << std::endl;
}