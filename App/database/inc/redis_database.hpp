/**
 * @file redis_database.hpp
 * @brief Implements RedisDatabaseHandler for Redis-based database operations.
 */

#pragma once
#include "inc/database_interface.hpp"
#include <cpp_redis/cpp_redis>
#include <string>
#include <nlohmann/json.hpp>

/// @class RedisDatabaseHandler
/// @brief Singleton class for interacting with a Redis database backend.
/// @details Provides methods to save, retrieve, update, remove, etc JSON objects in Redis.
/// Implements the IDatabaseHandler interface for backend-agnostic database operations.
class RedisDatabaseHandler : public IDatabaseHandler {
public:
    /**
     * @brief Retrieves the singleton instance of the RedisDatabaseHandler.
     * @return Reference to the singleton RedisDatabaseHandler.
     */
    static RedisDatabaseHandler& GetInstance();

    /**
     * @brief Saves a JSON object to Redis with the specified key.
     * @param key The key under which the JSON object will be stored.
     * @param value The JSON object to store.
     */
    void SaveJson(const std::string& key, const nlohmann::json& value) override;
    
    /**
     * @brief Retrieves a JSON object from Redis by key.
     * @param key The key of the JSON object to retrieve.
     * @return The JSON object associated with the key, or an empty object if not found.
     */
    nlohmann::json GetJson(const std::string& key) override;

    /**
     * @brief Clears all data from the Redis database.
     */
    void ClearDatabase() override;

    /**
     * @brief Updates a specific field in the JSON object stored under the given key in Redis.
     * @param key The key of the JSON object to update.
     * @param field The field within the JSON object to update.
     * @param value The new value for the specified field.
     */
    void UpdateField(const std::string& key, const std::string& field, const std::string& value) override;
    
    /**
     * @brief Removes the JSON object associated with the specified key from Redis.
     * @param key The key of the JSON object to remove.
     */
    void RemoveKey(const std::string& key) override;

private:
    /// @brief Constructs a RedisDatabaseHandler and connects to the Redis server.
    RedisDatabaseHandler();
    
    /// @brief Destructor for RedisDatabaseHandler. Disconnects from the Redis server.
    ~RedisDatabaseHandler();

    /// @brief Deleted copy constructor to prevent copying.
    RedisDatabaseHandler(const RedisDatabaseHandler&) = delete;

    /// @brief Deleted copy assignment operator to prevent assignment.
    RedisDatabaseHandler& operator=(const RedisDatabaseHandler&) = delete;

    /// @brief Redis client instance for database operations.
    cpp_redis::client redis_;
};