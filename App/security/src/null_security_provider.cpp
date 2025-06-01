#include "inc/null_security_provider.hpp"

bool NullSecurityProvider::Decrypt(const std::string& input, std::string& output) {
    output = input;  // Pass through without decryption
    return true;
}