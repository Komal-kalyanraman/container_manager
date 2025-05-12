#include "inc/cm_server_handler.hpp"

#include <iostream>
#include <thread>

int main() {
    auto& server = ServerHandler::GetInstance();
    server.StartCmServer();

    // Keep main alive for demonstration (or run your actual logic)
    std::cin.get();
    return 0;
}