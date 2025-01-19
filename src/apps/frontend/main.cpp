#include <iostream>
#include "HttpServer.h"
#include "TaskManager.h"
#include "config.h"
#include "eurora/utils/logger.h"

int main(int argc, char* argv[]) {
    try {
        // Parse command-line arguments and configuration file
        Config config = Config::Parse(argc, argv);

        // Display parsed configuration
        std::cout << "Configuration:" << std::endl;
        std::cout << "  Storage Directory: " << config.storage_dir << std::endl;
        std::cout << "  Port: " << config.port << std::endl;
        std::cout << "  Backend Instances: " << config.backend_instances << std::endl;

        // TODO: Initialize server and start handling tasks
        Logger::Instance().InitFromConfig(config.log_config);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
