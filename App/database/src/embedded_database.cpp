/**
 * @file embedded_database.cpp
 * @brief Implements the EmbeddedDatabaseHandler class for a lightweight, fixed-size in-memory database.
 *
 * This file provides a simple, embedded-friendly implementation of the IDatabaseHandler interface.
 * It uses a fixed-size array to store key-value pairs, where keys are fixed-length character arrays
 * and values are nlohmann::json objects. This avoids dynamic memory allocation for keys and limits
 * the number of entries, making it suitable for resource-constrained embedded systems.
 */

#include "inc/embedded_database.hpp"
#include <cstring>

/**
 * @brief Constructs an EmbeddedDatabaseHandler and initializes all entries as unused.
 */
EmbeddedDatabaseHandler::EmbeddedDatabaseHandler() {
    for (auto& e : entries_) e.used = false;
}

/**
 * @brief Saves a JSON object to the database with the specified key.
 *        If the key exists, its value is updated. If not, a new entry is created.
 *        If the database is full, the operation is ignored.
 * @param key The key under which the JSON object will be stored.
 * @param value The JSON object to store.
 */
void EmbeddedDatabaseHandler::SaveJson(const std::string& key, const nlohmann::json& value) {
    // Update existing entry if key exists
    for (auto& e : entries_) {
        if (e.used && std::strncmp(e.key, key.c_str(), kMaxKeyLen) == 0) {
            e.value = value;
            return;
        }
    }
    // Add new entry if space is available
    for (auto& e : entries_) {
        if (!e.used) {
            std::strncpy(e.key, key.c_str(), kMaxKeyLen);
            e.key[kMaxKeyLen-1] = '\0';
            e.value = value;
            e.used = true;
            return;
        }
    }
    // No space left: in production, handle this gracefully!
}

/**
 * @brief Retrieves a JSON object from the database by key.
 * @param key The key of the JSON object to retrieve.
 * @return The JSON object associated with the key, or an empty object if not found.
 */
nlohmann::json EmbeddedDatabaseHandler::GetJson(const std::string& key) {
    for (const auto& e : entries_) {
        if (e.used && std::strncmp(e.key, key.c_str(), kMaxKeyLen) == 0) {
            return e.value;
        }
    }
    return nlohmann::json{};
}

/**
 * @brief Clears all data from the database, marking all entries as unused.
 */
void EmbeddedDatabaseHandler::ClearDatabase() {
    for (auto& e : entries_) e.used = false;
}

/**
 * @brief Updates a specific field in the JSON object stored under the given key.
 *        If the key does not exist, the operation is ignored.
 * @param key The key of the JSON object to update.
 * @param field The field within the JSON object to update.
 * @param value The new value for the specified field.
 */
void EmbeddedDatabaseHandler::UpdateField(const std::string& key, const std::string& field, const std::string& value) {
    for (auto& e : entries_) {
        if (e.used && std::strncmp(e.key, key.c_str(), kMaxKeyLen) == 0) {
            e.value[field] = value;
            return;
        }
    }
}

/**
 * @brief Removes the JSON object associated with the specified key from the database.
 *        Marks the entry as unused.
 * @param key The key of the JSON object to remove.
 */
void EmbeddedDatabaseHandler::RemoveKey(const std::string& key) {
    for (auto& e : entries_) {
        if (e.used && std::strncmp(e.key, key.c_str(), kMaxKeyLen) == 0) {
            e.used = false;
            return;
        }
    }
}