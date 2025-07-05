#pragma once

#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <httplib.h>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
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
    explicit HeartbeatServer(const HeartbeatConfig& cfg);
    ~HeartbeatServer();

    Status Start();
    Status Stop();
    bool IsRunning() const;

private:
    void SetupRoutes();
    void ServerLoop();

    Status HandleLiveness(const httplib::Request& req, httplib::Response& res);
    Status HandleHealth(const httplib::Request& req, httplib::Response& res);

    long GetUptimeSeconds() const;

    Status BuildHealthReport(nlohmann::json& out_json) const;

    HeartbeatConfig cfg_;
    std::unique_ptr<httplib::Server> server_;
    std::thread server_thread_;
    std::atomic<bool> running_{false};
    std::chrono::steady_clock::time_point start_time_;
};