/**
 * @file init_handler.hpp
 * @brief Declares project-wide initialization functions.
 */

/**
 * @brief Initializes logging and clears the database before server startup.
 * @details This function should be called at application startup to set up logging and ensure the database is in a clean state.
 */

#pragma once

/// @brief Initializes logging.
void InitLogging();

/// @brief Initializes and clears the database.
void InitDatabase();

/// @brief Clears the POSIX message queue.
void InitMessageQueue();

/// @brief Clears retained MQTT messages.
void InitMqttRetainedMessages();

/// @brief Initializes all enabled project subsystems.
void InitProject();