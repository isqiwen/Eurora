#include "config.h"

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>

namespace eurora::fe {

Config Config::Parse(int argc, char* argv[]) {
    cxxopts::Options options("Eurora", "Manage and monitor MRI reconstruction tasks");

    options.add_options()("h,help", "Print usage");
    options.add_options()("c,config", "Path to the configuration file", cxxopts::value<std::string>());
    options.add_options()("l,log_config", "Path to logger configuration file", cxxopts::value<std::string>());
    options.add_options()("s,storage_dir", "Set storage directory", cxxopts::value<std::string>()->default_value("./storage"));
    options.add_options()("p,port", "Set server port", cxxopts::value<unsigned short>()->default_value("9002"));
    options.add_options()("b,backend_instances", "Number of backend instances", cxxopts::value<unsigned int>()->default_value("1"));

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        std::exit(0);
    }

    Config config;
    config.storage_dir       = result["storage_dir"].as<std::string>();
    config.log_config        = result["log_config"].as<std::string>();
    config.port              = result["port"].as<unsigned short>();
    config.backend_instances = result["backend_instances"].as<unsigned int>();

    if (result.count("config")) {
        auto file_config = Config::LoadFromFile(result["config"].as<std::string>());
        // Merge file_config into config (command-line takes priority)
        if (!result.count("storage_dir")) {
            config.storage_dir = file_config.storage_dir;
        }
        if (!result.count("log_config")) {
            config.log_config = file_config.log_config;
        }
        if (!result.count("port")) {
            config.port = file_config.port;
        }
        if (!result.count("backend_instances")) {
            config.backend_instances = file_config.backend_instances;
        }
    }

    return config;
}

Config Config::LoadFromFile(const std::string& filename) {
    Config config;
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open configuration file: " + filename);
    }

    nlohmann::json json;
    file >> json;

    if (json.contains("storage_dir")) {
        config.storage_dir = json["storage_dir"];
    }
    if (json.contains("port")) {
        config.port = json["port"];
    }
    if (json.contains("backend_instances")) {
        config.backend_instances = json["backend_instances"];
    }

    return config;
}

}  // namespace eurora::fe
