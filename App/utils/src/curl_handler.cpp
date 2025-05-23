#include "inc/curl_handler.hpp"
#include <curl/curl.h>

/**
 * @brief Default constructor for CurlHandler.
 */
CurlHandler::CurlHandler() {}

/**
 * @brief Destructor for CurlHandler.
 */
CurlHandler::~CurlHandler() {}

/**
 * @brief Callback function for libcurl to write received data into a std::string.
 * 
 * @param contents Pointer to the received data.
 * @param size Size of each data element.
 * @param nmemb Number of data elements.
 * @param userp Pointer to the user data (std::string*).
 * @return Number of bytes processed.
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::string* str = static_cast<std::string*>(userp);
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

/**
 * @brief Perform a HTTP GET request over a Unix domain socket.
 * 
 * @param url The HTTP URL to request (e.g., http://localhost/_ping).
 * @param unix_socket The path to the Unix socket (e.g., /var/run/docker.sock).
 * @param response Output parameter to store the response body.
 * @return true if the request was successful, false otherwise.
 */
bool CurlHandler::GetUnix(const std::string& url, const std::string& unix_socket, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, unix_socket.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}

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
bool CurlHandler::PostUnix(const std::string& url, const std::string& unix_socket, const std::string& body, std::string& response, const std::string& content_type) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, ("Content-Type: " + content_type).c_str());

    curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, unix_socket.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}

/**
 * @brief Perform a HTTP DELETE request over a Unix domain socket.
 * 
 * @param url The HTTP URL to request.
 * @param unix_socket The path to the Unix socket.
 * @param response Output parameter to store the response body.
 * @return true if the request was successful, false otherwise.
 */
bool CurlHandler::DeleteUnix(const std::string& url, const std::string& unix_socket, std::string& response) {
    CURL* curl = curl_easy_init();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, unix_socket.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return (res == CURLE_OK);
}