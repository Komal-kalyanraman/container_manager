#pragma once
#include <string>

/**
 * @brief CurlHandler provides simple HTTP methods over Unix domain sockets using libcurl.
 * 
 * This class is designed to work with both Docker and Podman APIs, allowing you to send
 * HTTP requests (GET, POST, DELETE) over a Unix socket.
 */
class CurlHandler {
public:
    /**
     * @brief Construct a new CurlHandler object.
     */
    CurlHandler();

    /**
     * @brief Destroy the CurlHandler object.
     */
    ~CurlHandler();

    /**
     * @brief Perform a HTTP GET request over a Unix domain socket.
     * 
     * @param url The HTTP URL to request (e.g., http://localhost/_ping).
     * @param unix_socket The path to the Unix socket (e.g., /var/run/docker.sock).
     * @param response Output parameter to store the response body.
     * @return true if the request was successful, false otherwise.
     */
    bool GetUnix(const std::string& url, const std::string& unix_socket, std::string& response);

    /**
     * @brief Perform a HTTP POST request over a Unix domain socket.
     * 
     * @param url The HTTP URL to request.
     * @param unix_socket The path to the Unix socket.
     * @param body The request body to send (usually JSON).
     * @param response Output parameter to store the response body.
     * @param content_type The Content-Type header value (default: application/json).
     * @return true if the request was successful, false otherwise.
     */
    bool PostUnix(const std::string& url, const std::string& unix_socket, const std::string& body, std::string& response, const std::string& content_type = "application/json");

    /**
     * @brief Perform a HTTP DELETE request over a Unix domain socket.
     * 
     * @param url The HTTP URL to request.
     * @param unix_socket The path to the Unix socket.
     * @param response Output parameter to store the response body.
     * @return true if the request was successful, false otherwise.
     */
    bool DeleteUnix(const std::string& url, const std::string& unix_socket, std::string& response);
};