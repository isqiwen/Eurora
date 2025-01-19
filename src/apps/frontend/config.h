#pragma once

#include <string>

namespace eurora::fe {

struct Config {
    std::string storage_dir        = "./storage";
    std::string log_config         = "./log_config.json";
    unsigned short port            = 9002;
    unsigned int backend_instances = 1;

    static Config Parse(int argc, char* argv[]);
    static Config LoadFromFile(const std::string& filename);
};

}  // namespace eurora::fe
