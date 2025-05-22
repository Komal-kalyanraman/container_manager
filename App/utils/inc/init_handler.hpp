/**
 * @file init_handler.hpp
 * @brief Declares project-wide initialization functions.
 */

#pragma once

/**
 * @brief Initializes logging.
 * @details Sets up the logging system for the application.
 */
void InitLogging();

/**
 * @brief Initializes and clears the database.
 * @details Prepares the database for use and ensures it is in a clean state.
 */
void InitDatabase();

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
 */
void InitProject();