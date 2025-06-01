/**
 * @file null_security_provider.cpp
 * @brief Implementation of the NullSecurityProvider class for the "no encryption" case.
 *
 * This class provides a no-op Decrypt method, simply passing the input to the output.
 * Used when encryption is disabled at build time (ENABLE_ENCRYPTION=OFF).
 */

#include "inc/null_security_provider.hpp"

/**
 * @brief No-op decryption; simply returns the input as output.
 * @param input The input data (plain).
 * @param output The output data (same as input).
 * @return Always true.
 */
bool NullSecurityProvider::Decrypt(const std::string& input, std::string& output) {
    output = input;  // Pass through without decryption
    return true;
}