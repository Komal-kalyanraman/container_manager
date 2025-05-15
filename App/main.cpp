#include <memory>

#include <glog/logging.h>

#include "inc/http_server.hpp"
#include "inc/init_handler.hpp"
#include "inc/json_request_executor.hpp"

int main(int argc, char* argv[]) {
    InitProject();
    auto executor = std::make_shared<JsonRequestExecutorHandler>();
    HttpServerHandler server(executor);
    server.Start(kHttpServerPort);
    google::ShutdownGoogleLogging();
    return 0;
}