/**
 * @file init_handler.hpp
 * @brief Declares project-wide initialization functions for the Container Manager application.
 *
 * Provides initialization routines for logging, database, message queue, and MQTT subsystems.
 * All functions are designed to be called at application startup to ensure a clean and ready state.
 * The database initialization uses dependency injection for flexibility and testability.
 */

#pragma once

#include "inc/database_interface.hpp"

/**
 * @brief Initializes logging.
 * @details Sets up the logging system (e.g., Google glog) for the application.
 */
void InitLogging();

/**
 * @brief Initializes and clears the database.
 * @details Prepares the database for use and ensures it is in a clean state.
 * @param db Reference to an IDatabaseHandler implementation.
 */
void InitDatabase(IDatabaseHandler& db);

/**
 * @brief Clears the POSIX message queue.
 * @details Removes all messages from the POSIX message queue at startup.
 */
void InitMessageQueue();

/**
 * @brief Clears retained MQTT messages.
 * @details Removes all retained messages from the MQTT broker at startup.
 */
void InitMqttRetainedMessages();

/**
 * @brief Initializes all enabled project subsystems.
 * @details Calls all necessary initialization routines for logging, database, message queue, and MQTT.
 * @param db Reference to an IDatabaseHandler implementation.
 */
void InitProject(IDatabaseHandler& db);