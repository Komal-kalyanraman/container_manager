/**
 * @file database_interface.hpp
 * @brief Abstract interface for database operations.
 */

#pragma once

#include <string>
#include <nlohmann/json.hpp>

/**
 * @class IDatabaseHandler
 * @brief Interface for database handlers supporting JSON storage and retrieval.
 * @details This interface defines the contract for database operations such as saving, retrieving,
 * updating, and removing JSON objects by key. Implementations can support different backend databases.
 */
class IDatabaseHandler {
public:
    /**
     * @brief Virtual destructor for safe polymorphic destruction.
     */
    virtual ~IDatabaseHandler() = default;
    
    /**
     * @brief Saves a JSON object to the database with the specified key.
     * @param key The key under which the JSON object will be stored.
     * @param value The JSON object to store.
     */
    virtual void SaveJson(const std::string& key, const nlohmann::json& value) = 0;
    
    /**
     * @brief Retrieves a JSON object from the database by key.
     * @param key The key of the JSON object to retrieve.
     * @return The JSON object associated with the key, or an empty object if not found.
     */
    virtual nlohmann::json GetJson(const std::string& key) = 0;
    
    /**
     * @brief Clears all data from the database.
     */
    virtual void ClearDatabase() = 0;
    
    /**
     * @brief Updates a specific field in the JSON object stored under the given key.
     * @param key The key of the JSON object to update.
     * @param field The field within the JSON object to update.
     * @param value The new value for the specified field.
     */
    virtual void UpdateField(const std::string& key, const std::string& field, const std::string& value) = 0;
    
    /**
     * @brief Removes the JSON object associated with the specified key from the database.
     * @param key The key of the JSON object to remove.
     */
    virtual void RemoveKey(const std::string& key) = 0;
};