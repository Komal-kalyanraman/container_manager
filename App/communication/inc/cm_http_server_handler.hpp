#pragma once
#include <memory>

#include "inc/cm_common.hpp"
#include "inc/cm_json_requestexecutor_handler.hpp"

class HttpServerHandler {
public:
    explicit HttpServerHandler(std::shared_ptr<RequestExecutor> executor);
    void Start(int port);
private:
    std::shared_ptr<RequestExecutor> executor_;
};