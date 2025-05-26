/**
 * @file redis_database.hpp
 * @brief Declares the RedisDatabaseHandler class for Redis-based database operations.
 *
 * This class implements the IDatabaseHandler interface and provides methods to save,
 * retrieve, update, and remove JSON objects in a Redis database. It is designed for
 * dependency injection—create and inject instances as needed.
 */

#pragma once
#include "inc/database_interface.hpp"
#include <cpp_redis/cpp_redis>
#include <string>
#include <nlohmann/json.hpp>

/**
 * @class RedisDatabaseHandler
 * @brief Class for interacting with a Redis database backend.
 * @details Implements the IDatabaseHandler interface for backend-agnostic database operations.
 *          Provides methods to save, retrieve, update, and remove JSON objects in Redis.
 *          Designed for direct instantiation and dependency injection.
 */
class RedisDatabaseHandler : public IDatabaseHandler {
public:
    /**
     * @brief Constructs a RedisDatabaseHandler and connects to the Redis server.
     */
    RedisDatabaseHandler();

    /**
     * @brief Destructor for RedisDatabaseHandler. Disconnects from the Redis server.
     */
    ~RedisDatabaseHandler();

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

    /**
     * @brief Deleted copy constructor to prevent copying.
     */
    RedisDatabaseHandler(const RedisDatabaseHandler&) = delete;

    /**
     * @brief Deleted copy assignment operator to prevent assignment.
     */
    RedisDatabaseHandler& operator=(const RedisDatabaseHandler&) = delete;

private:
    /**
     * @brief Redis client instance for database operations.
     */
    cpp_redis::client redis_;
};