#pragma once
#include <string>

class CurlHandler {
public:
    CurlHandler();
    ~CurlHandler();

    // Performs a GET request to the given URL and stores the response in 'response'.
    // Returns true on success, false on failure.
    bool Get(const std::string& url, std::string& response);
};