// 
// This file is used to enable or disable logging using glog.
// It uses preprocessor directives to include the glog library and define macros for logging.
// If ENABLE_GLOG_LOGGING is defined, it includes the glog library and defines macros for logging.
// If ENABLE_GLOG_LOGGING is not defined, it defines the macros to print statements to std::cout.
// This allows for easy switching between glog and std::cout for logging purposes.
//
#pragma once

// Uncomment below line to enable glog logging
#define ENABLE_GLOG_LOGGING

#ifdef ENABLE_GLOG_LOGGING
#include <glog/logging.h>
#define CM_LOG_INFO LOG(INFO)
#define CM_LOG_WARN LOG(WARNING)
#define CM_LOG_FATAL LOG(FATAL)
#define CM_LOG_ERROR LOG(ERROR)
#else
#include <iostream>
#define CM_LOG_INFO if (false) std::cout
#define CM_LOG_WARN if (false) std::cout
#define CM_LOG_FATAL if (false) std::cout
#define CM_LOG_ERROR if (false) std::cout
#endif