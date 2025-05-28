/**
 * @file logging.hpp
 * @brief Logging macros and configuration for the Container Manager.
 *
 * @details
 * This file provides a mechanism to enable or disable logging using Google glog.
 * If ENABLE_GLOG_LOGGING is defined, it includes the glog library and defines macros for logging.
 * If ENABLE_GLOG_LOGGING is not defined, it defines the macros to use std::cout (or disables logging).
 * This allows for easy switching between glog and std::cout for logging purposes.
 */

#pragma once

// Uncomment below line to enable glog logging
#define ENABLE_GLOG_LOGGING

#ifdef ENABLE_GLOG_LOGGING
#include <glog/logging.h>
/// @brief Macro for logging informational messages using glog.
#define CM_LOG_INFO LOG(INFO)
/// @brief Macro for logging warning messages using glog.
#define CM_LOG_WARN LOG(WARNING)
/// @brief Macro for logging fatal error messages using glog.
#define CM_LOG_FATAL LOG(FATAL)
/// @brief Macro for logging error messages using glog.
#define CM_LOG_ERROR LOG(ERROR)
#else
#include <iostream>
/// @brief Macro for logging informational messages (no-op if glog is not enabled).
#define CM_LOG_INFO if (false) std::cout
/// @brief Macro for logging warning messages (no-op if glog is not enabled).
#define CM_LOG_WARN if (false) std::cerr
/// @brief Macro for logging fatal error messages (no-op if glog is not enabled).
#define CM_LOG_FATAL if (false) std::cerr
/// @brief Macro for logging error messages (no-op if glog is not enabled).
#define CM_LOG_ERROR if (false) std::cerr
#endif