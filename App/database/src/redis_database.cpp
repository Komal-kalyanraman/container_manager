/**
 * @file redis_database.cpp
 * @brief Implements RedisDatabaseHandler for Redis-based database operations.
 *
 * This file provides the implementation for connecting to a Redis server and performing
 * CRUD operations on JSON objects, including saving, retrieving, updating fields, and removing keys.
 * The class is designed for direct instantiation and dependency injection.
 */

#include "inc/redis_database.hpp"
#include <iostream>

/**
 * @brief Constructs a RedisDatabaseHandler and connects to the Redis server.
 */
RedisDatabaseHandler::RedisDatabaseHandler() {
    // Connect to Redis server (adjust host/port as needed)
    redis_.connect();
}

/**
 * @brief Destructor for RedisDatabaseHandler. Disconnects from the Redis server.
 */
RedisDatabaseHandler::~RedisDatabaseHandler() {
    redis_.disconnect();
}

/**
 * @brief Saves a JSON object to Redis with the specified key.
 * @param key The key under which the JSON object will be stored.
 * @param value The JSON object to store.
 */
void RedisDatabaseHandler::SaveJson(const std::string& key, const nlohmann::json& value) {
    // Save JSON as string
    std::string json_str = value.dump();
    redis_.set(key, json_str);
    redis_.sync_commit();
}

/**
 * @brief Retrieves a JSON object from Redis by key.
 * @param key The key of the JSON object to retrieve.
 * @return The JSON object associated with the key, or an empty object if not found.
 */
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

/**
 * @brief Clears all data from the Redis database.
 */
void RedisDatabaseHandler::ClearDatabase() {
    redis_.flushdb();
    redis_.sync_commit();
}

/**
 * @brief Updates a specific field in the JSON object stored under the given key in Redis.
 * @param key The key of the JSON object to update.
 * @param field The field within the JSON object to update.
 * @param value The new value for the specified field.
 */
void RedisDatabaseHandler::UpdateField(const std::string& key, const std::string& field, const std::string& value) {
    nlohmann::json obj = GetJson(key);
    if (obj.is_null()) {
        std::cerr << "[RedisDatabaseHandler] Cannot update field, key not found: " << key << std::endl;
        return;
    }
    obj[field] = value;
    SaveJson(key, obj);
}

/**
 * @brief Removes the JSON object associated with the specified key from Redis.
 * @param key The key of the JSON object to remove.
 */
void RedisDatabaseHandler::RemoveKey(const std::string& key) {
    redis_.del({key});
    redis_.sync_commit();
}