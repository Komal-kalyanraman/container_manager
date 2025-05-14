#include <memory>
#include <glog/logging.h>

#include "inc/cm_http_server_handler.hpp"
#include "inc/cm_json_requestexecutor_handler.hpp"

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    auto executor = std::make_shared<JsonRequestExecutorHandler>();
    HttpServerHandler server(executor);
    server.Start(kHttpServerPort);
    google::ShutdownGoogleLogging();
    return 0;
}