#pragma once
#include <memory>

#include "inc/common.hpp"
#include "inc/threadpool.hpp"
#include "inc/json_request_executor.hpp"

class HttpServerHandler {
public:
    explicit HttpServerHandler(std::shared_ptr<RequestExecutor> executor);
    void Start(int port);
private:
    std::shared_ptr<RequestExecutor> executor_;
    std::unique_ptr<ThreadPool> pool_;
};