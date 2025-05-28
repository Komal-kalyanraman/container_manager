#pragma once
#include <string>

/**
 * @file status.hpp
 * @brief Defines status codes and a Status struct for standardized error handling.
 *
 * This file provides a generic Status type and StatusCode enumeration for use across
 * the application. It enables consistent error reporting and handling in all modules.
 */

/**
 * @enum StatusCode
 * @brief Enumerates possible status codes for operation results.
 */
enum class StatusCode {
    Success,           ///< Operation completed successfully.
    NotFound,          ///< Requested item was not found.
    InvalidArgument,   ///< An invalid argument was provided.
    InternalError,     ///< An internal error occurred.
    Timeout,           ///< Operation timed out.
    PermissionDenied,  ///< Operation not permitted.
    AlreadyExists,     ///< Item already exists.
    Unavailable,       ///< Resource or service is unavailable.
    // ...add more as needed
};

/**
 * @struct Status
 * @brief Represents the result of an operation, including a code and message.
 */
struct Status {
    StatusCode code;      ///< The status code.
    std::string message;  ///< A human-readable message describing the status.

    /**
     * @brief Constructs a Status with the given code and message.
     * @param c The status code.
     * @param msg The status message (default: "OK").
     */
    Status(StatusCode c = StatusCode::Success, std::string msg = "OK")
        : code(c), message(std::move(msg)) {}

    /**
     * @brief Returns a success Status.
     * @return Status with code Success and message "OK".
     */
    static Status Ok() { return {StatusCode::Success, "OK"}; }

    /**
     * @brief Returns an error Status with the given code and message.
     * @param code The error status code.
     * @param msg The error message.
     * @return Status with the specified code and message.
     */
    static Status Error(StatusCode code, const std::string& msg) { return {code, msg}; }

    /**
     * @brief Checks if the status represents success.
     * @return true if status code is Success, false otherwise.
     */
    bool ok() const { return code == StatusCode::Success; }
};