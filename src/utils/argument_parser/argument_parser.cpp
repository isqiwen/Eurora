#include "argument_parser.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "eurora/utils/logger.h"
#include "utils/common/string_utils.h"

namespace eurora::utils {

void ArgumentParser::AddFlag(const std::string& name, const std::string& description) {
    arguments_[name] = {typeid(bool), description, false, true, true, false, Source::kDefaultValue};
}

void ArgumentParser::ParseCommandLine(const std::vector<std::string>& argv) {
    for (const auto& arg : argv) {
        if (std::find(help_flag_keys_.begin(), help_flag_keys_.end(), arg) != help_flag_keys_.end()) {
            throw CallForHelp("User requested help");
        }
    }

    for (int i = 0; i < argv.size(); ++i) {
        std::string arg = argv[i];
        if (StringUtils::StartsWith(arg, "--")) {
            std::string argKey = arg.substr(2);
            if (arguments_.count(argKey)) {
                auto& argument = arguments_[argKey];
                if (argument.is_flag_) {
                    argument.value_  = true;
                    argument.source_ = Source::kCommandLine;
                } else if (i + 1 < argv.size()) {
                    i++;

                    if (argument.type_ == typeid(int)) {
                        argument.value_ = std::stoi(argv[i]);
                    } else if (argument.type_ == typeid(std::string)) {
                        argument.value_ = argv[i];
                    } else if (argument.type_ == typeid(double)) {
                        argument.value_ = std::stod(argv[i]);
                    } else if (argument.type_ == typeid(bool)) {
                        argument.value_ = (argv[i] == "true" || argv[i] == "True" || argv[i] == "TRUE" || argv[i] == "1");
                    } else if (argument.type_ == typeid(float)) {
                        argument.value_ = std::stof(argv[i]);
                    } else {
                        EURORA_THROW_EXCEPTION(ErrorCode::kInvalidArgument, "Argument : " + argKey + " is not valid type!");
                    }

                    argument.source_ = Source::kCommandLine;
                }
            }
        }
    }
}

void ArgumentParser::ParseConfigFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        EURORA_THROW_EXCEPTION(ErrorCode::kOperationFailed, "Unable to open configuration file: " + filename);
    }
    nlohmann::json config;
    file >> config;

    for (const auto& [key, value] : config.items()) {
        if (arguments_.count(key)) {
            auto& argument = arguments_[key];
            if (argument.config_allowed_ && argument.source_ != Source::kCommandLine) {
                if (argument.type_ == typeid(int)) {
                    argument.value_ = value.get<int>();
                } else if (argument.type_ == typeid(std::string)) {
                    argument.value_ = value.get<std::string>();
                } else if (argument.type_ == typeid(double)) {
                    argument.value_ = value.get<double>();
                } else if (argument.type_ == typeid(bool)) {
                    argument.value_ = value.get<bool>();
                } else if (argument.type_ == typeid(float)) {
                    argument.value_ = value.get<float>();
                } else {
                    EURORA_THROW_EXCEPTION(ErrorCode::kInvalidArgument, "Argument : " + key + " is not valid type!");
                }

                argument.source_ = Source::kConfigFile;
            }
        }
    }
}

void ArgumentParser::Validate() const {
    for (const auto& [name, arg] : arguments_) {
        if (arg.required_ && arg.source_ == Source::kNone) {
            EURORA_THROW_EXCEPTION(ErrorCode::kInvalidArgument, "Missing required argument: " + name);
        }
    }
}

void ArgumentParser::AddHelpFlagKey(const std::string& helpFlagKey) {
    if (StringUtils::StartsWith(helpFlagKey, "-")) {
        help_flag_keys_.push_back(helpFlagKey);
    } else {
        if (helpFlagKey.length() > 1) {
            help_flag_keys_.push_back("--" + helpFlagKey);
        } else if (helpFlagKey.length() == 1) {
            help_flag_keys_.push_back("-" + helpFlagKey);
        }
    }
}

void ArgumentParser::Help() const {
    STREAM_INFO() << "Available Options:\n";

    for (const auto& [name, arg] : arguments_) {
        std::stringstream info;
        info << "--" << std::setw(15) << std::left << name << " : ";
        info << (arg.is_flag_ ? "[flag]" : "[option]");

        if (arg.required_) {
            info << " [required]";
        } else {
            if (!arg.is_flag_) {
                info << " [default: ";

                if (arg.type_ == typeid(int)) {
                    info << std::get<int>(arg.value_);
                } else if (arg.type_ == typeid(bool)) {
                    info << (std::get<bool>(arg.value_) ? "true" : "false");
                } else if (arg.type_ == typeid(double)) {
                    info << std::get<double>(arg.value_);
                } else if (arg.type_ == typeid(std::string)) {
                    info << std::get<std::string>(arg.value_);
                } else if (arg.type_ == typeid(float)) {
                    info << std::get<float>(arg.value_);
                } else {
                    EURORA_THROW_EXCEPTION(ErrorCode::kInvalidArgument, "Argument : " + name + " is not valid type!");
                }

                info << "]";

                STREAM_INFO() << info.str();
            }
        }

        STREAM_INFO() << "\n " << arg.description_ << "\n\n";
    }
}

void ArgumentParser::PrintParsedArguments() const {
    STREAM_INFO() << "Parsed Arguments:\n";

    for (const auto& [name, arg] : arguments_) {
        std::stringstream info;
        info << "--" << name << " : ";
        if (arg.source_ == Source::kCommandLine) {
            info << "[CommandLine] ";
        } else if (arg.source_ == Source::kConfigFile) {
            info << "[ConfigFile] ";
        } else if (arg.source_ == Source::kDefaultValue) {
            info << "[DefaultValue] ";
        } else {
            info << "[None] ";
        }

        if (arg.type_ == typeid(int)) {
            info << std::get<int>(arg.value_);
        } else if (arg.type_ == typeid(float)) {
            info << std::get<float>(arg.value_);
        } else if (arg.type_ == typeid(double)) {
            info << std::get<double>(arg.value_);
        } else if (arg.type_ == typeid(bool)) {
            info << (std::get<bool>(arg.value_) ? "true" : "false");
        } else if (arg.type_ == typeid(std::string)) {
            info << std::get<std::string>(arg.value_);
        }

        STREAM_INFO() << info.str();
    }
}

}  // namespace eurora::utils
