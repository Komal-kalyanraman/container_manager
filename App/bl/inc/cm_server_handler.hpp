#pragma once

#include <thread>

/**
 * @brief ServerHandler is used to create a REST server and handle requests.
 *        Implements the singleton pattern.
 */
class ServerHandler
{
private:
    ServerHandler() = default;
    ~ServerHandler() = default;

    // Rule of 5: Delete copy/move constructors and assignment operators
    ServerHandler(const ServerHandler&) = delete;               // Copy constructor
    ServerHandler& operator=(const ServerHandler&) = delete;    // Copy assignment operator
    ServerHandler(ServerHandler&&) = delete;                    // Move constructor
    ServerHandler& operator=(ServerHandler&&) = delete;         // Move assignment operator

    std::thread server_thread_;

    void ServerMainLoop(); // Actual server logic

public:
    /**
     * @brief Get the singleton instance of ServerHandler.
     * @return ServerHandler&
     */
    static ServerHandler& GetInstance();

    /**
     * @brief Start the Container Manager server in a separate thread.
     */
    void StartCmServer();
};