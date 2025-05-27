/**
 * @file embedded_database.hpp
 * @brief Declares the EmbeddedDatabaseHandler class for a lightweight, fixed-size in-memory database.
 *
 * This class provides an embedded-friendly implementation of the IDatabaseHandler interface.
 * It uses a fixed-size array to store key-value pairs, where keys are fixed-length character arrays
 * and values are nlohmann::json objects. This design avoids dynamic memory allocation for keys and
 * limits the number of entries, making it suitable for resource-constrained embedded systems.
 */

#pragma once

#include <array>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/database_interface.hpp"

/**
 * @class EmbeddedDatabaseHandler
 * @brief Lightweight, fixed-size in-memory key-value store for embedded systems.
 *
 * Implements the IDatabaseHandler interface using a fixed-size array of entries.
 * Each entry consists of a fixed-length key, a JSON value, and a usage flag.
 */
class EmbeddedDatabaseHandler : public IDatabaseHandler {
public:
    /**
     * @brief Constructs an EmbeddedDatabaseHandler and initializes all entries as unused.
     */
    EmbeddedDatabaseHandler();

    /**
     * @brief Saves a JSON object to the database with the specified key.
     *        Updates the value if the key exists, or creates a new entry if space is available.
     * @param key The key under which the JSON object will be stored.
     * @param value The JSON object to store.
     */
    void SaveJson(const std::string& key, const nlohmann::json& value) override;

    /**
     * @brief Retrieves a JSON object from the database by key.
     * @param key The key of the JSON object to retrieve.
     * @return The JSON object associated with the key, or an empty object if not found.
     */
    nlohmann::json GetJson(const std::string& key) override;

    /**
     * @brief Clears all data from the database, marking all entries as unused.
     */
    void ClearDatabase() override;

    /**
     * @brief Updates a specific field in the JSON object stored under the given key.
     *        If the key does not exist, the operation is ignored.
     * @param key The key of the JSON object to update.
     * @param field The field within the JSON object to update.
     * @param value The new value for the specified field.
     */
    void UpdateField(const std::string& key, const std::string& field, const std::string& value) override;

    /**
     * @brief Removes the JSON object associated with the specified key from the database.
     *        Marks the entry as unused.
     * @param key The key of the JSON object to remove.
     */
    void RemoveKey(const std::string& key) override;

private:
    /**
     * @struct Entry
     * @brief Represents a single key-value pair in the embedded database.
     */
    struct Entry {
        char key[kMaxKeyLen];      ///< Fixed-length key.
        nlohmann::json value;      ///< JSON value associated with the key.
        bool used;                 ///< Indicates if this entry is in use.
    };

    std::array<Entry, kMaxEntries> entries_; ///< Fixed-size array of database entries.
};