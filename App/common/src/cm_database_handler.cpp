#include "inc/cm_database_handler.hpp"

#include <iostream>

DatabaseHandler::DatabaseHandler() {
    // Connect to Redis server (adjust host/port as needed)
    redis_.connect();
}

DatabaseHandler::~DatabaseHandler() {
    redis_.disconnect();
}

DatabaseHandler& DatabaseHandler::GetInstance() {
    static DatabaseHandler instance;
    return instance;
}

void DatabaseHandler::SaveJson(const std::string& key, const nlohmann::json& value) {
    // Save JSON as string
    std::string json_str = value.dump();
    redis_.set(key, json_str);
    redis_.sync_commit();
    std::cout << "[DatabaseHandler] Saving to Redis: key=" << key << ", value=" << json_str << std::endl;
}

nlohmann::json DatabaseHandler::GetJson(const std::string& key) {
    auto reply = redis_.get(key);
    redis_.sync_commit();
    auto result = reply.get().as_string();
    return nlohmann::json::parse(result);
}