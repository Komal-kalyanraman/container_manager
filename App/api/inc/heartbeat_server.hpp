#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "inc/status.hpp"

/**
 * @file heartbeat_server.hpp
 * @brief Dedicated lightweight HTTP server for heartbeat and health endpoints.
 *
 * Runs independently of main REST API server. Always available for monitoring.
 * Reports only the health and status of the Container Manager process itself.
 */
class HeartbeatServer {
public:
    explicit HeartbeatServer(int port = 8090);
    ~HeartbeatServer();

    Status Start();
    Status Stop();
    bool IsRunning() const;

private:
    void SetupRoutes();
    void ServerLoop();

    Status HandlePing(const httplib::Request& req, httplib::Response& res);
    Status HandleHealth(const httplib::Request& req, httplib::Response& res);

    long GetUptimeSeconds() const;

    Status GetBasicHealth(nlohmann::json& out_json) const;

    std::unique_ptr<httplib::Server> server_;
    std::thread server_thread_;
    std::atomic<bool> running_{false};
    int port_;
    std::chrono::steady_clock::time_point start_time_;
};