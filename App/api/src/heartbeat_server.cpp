#include "inc/heartbeat_server.hpp"
#include "inc/logging.hpp"
#include <fstream>
#include <string>
#include <iostream>

HeartbeatServer::HeartbeatServer(int port)
    : port_(port), start_time_(std::chrono::steady_clock::now()) {
    server_ = std::make_unique<httplib::Server>();
    SetupRoutes();
}

HeartbeatServer::~HeartbeatServer() {
    Stop();
}

void HeartbeatServer::SetupRoutes() {
    server_->Get("/ping", [this](const httplib::Request& req, httplib::Response& res) {
        HandlePing(req, res);
    });

    server_->Get("/health", [this](const httplib::Request& req, httplib::Response& res) {
        HandleHealth(req, res);
    });

    server_->set_pre_routing_handler([](const httplib::Request& req, httplib::Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "*");
        if (req.method == "OPTIONS") {
            res.status = 200;
            return httplib::Server::HandlerResponse::Handled;
        }
        return httplib::Server::HandlerResponse::Unhandled;
    });
}

Status HeartbeatServer::Start() {
    if (running_) {
        CM_LOG_WARN << "Heartbeat server already running on port " << port_ << std::endl;
        return Status(StatusCode::AlreadyExists, "Heartbeat server already running");
    }
    running_ = true;
    server_thread_ = std::thread(&HeartbeatServer::ServerLoop, this);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    CM_LOG_INFO << "💓 Heartbeat Server started on port " << port_ << std::endl;
    return Status::Ok();
}

Status HeartbeatServer::Stop() {
    if (!running_) return Status(StatusCode::Unavailable, "Heartbeat server not running");
    CM_LOG_INFO << "Stopping Heartbeat Server..." << std::endl;
    running_ = false;
    if (server_) server_->stop();
    if (server_thread_.joinable()) server_thread_.join();
    CM_LOG_INFO << "💓 Heartbeat Server stopped" << std::endl;
    return Status::Ok();
}

void HeartbeatServer::ServerLoop() {
    try {
        if (!server_->listen("0.0.0.0", port_)) {
            CM_LOG_ERROR << "Failed to start Heartbeat Server on port " << port_ << std::endl;
            running_ = false;
        }
    } catch (const std::exception& e) {
        CM_LOG_ERROR << "Heartbeat Server error: " << e.what() << std::endl;
        running_ = false;
    }
}

Status HeartbeatServer::HandlePing(const httplib::Request&, httplib::Response& res) {
    nlohmann::json response = {
        {"status", "alive"},
        {"timestamp", std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count()}
    };
    res.set_content(response.dump(), "application/json");
    res.status = 200;
    return Status::Ok();
}

Status HeartbeatServer::HandleHealth(const httplib::Request&, httplib::Response& res) {
    nlohmann::json health;
    Status st = GetBasicHealth(health);
    if (st.ok()) {
        res.set_content(health.dump(2), "application/json");
        res.status = 200;
        return Status::Ok();
    } else {
        nlohmann::json error_response = {
            {"status", "error"},
            {"message", st.message},
            {"timestamp", std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()}
        };
        res.set_content(error_response.dump(), "application/json");
        res.status = 500;
        return st;
    }
}

long HeartbeatServer::GetUptimeSeconds() const {
    auto now = std::chrono::steady_clock::now();
    auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - start_time_);
    return uptime.count();
}

Status HeartbeatServer::GetBasicHealth(nlohmann::json& out_json) const {
    try {
        out_json = {
            {"status", "healthy"},
            {"timestamp", std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()},
            {"uptime_seconds", GetUptimeSeconds()},
            {"version", "0.7.2"},
            {"service", "container_manager"},
            {"heartbeat_port", port_},
            {"features", {
#if ENABLE_REST
                {"rest_enabled", true},
#else
                {"rest_enabled", false},
#endif
#if ENABLE_MQTT
                {"mqtt_enabled", true},
#else
                {"mqtt_enabled", false},
#endif
#if ENABLE_DBUS
                {"dbus_enabled", true},
#else
                {"dbus_enabled", false},
#endif
#if ENABLE_ENCRYPTION
                {"encryption_enabled", true},
#else
                {"encryption_enabled", false},
#endif
            }}
        };
        return Status::Ok();
    } catch (const std::exception& e) {
        return Status(StatusCode::InternalError, e.what());
    }
}

bool HeartbeatServer::IsRunning() const {
    return running_;
}