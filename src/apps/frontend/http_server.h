#pragma once
#include <string>
#include "TaskManager.h"

namespace eurora::fe {

class HttpServer {
public:
    HttpServer(unsigned short port, TaskManager& task_manager);
    void start();
    void stop();

private:
    unsigned short port_;
    TaskManager& task_manager_;
    std::atomic<bool> running_;
};
}  // namespace eurora::fe
