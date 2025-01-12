#include "ArgumentParser.hpp"

#include <sstream>
#include <fstream>
#include <iomanip>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "common/logger/Logger.hpp"


namespace eurora::common {

    void ArgumentParser::addFlag(const std::string& name, const std::string& description) {
        mArguments[name] = {
            typeid(bool),
            description,
            false,
            true,
            true,
            false,
            Source::DefaultValue
        };
    }

    void ArgumentParser::parseCommandLine(const std::vector<std::string>& argv) {
        for (const auto& arg : argv) {
            if (std::find(mHelpFlagKeys.begin(), mHelpFlagKeys.end(), arg) != mHelpFlagKeys.end()) {
                throw CallForHelp("User requested help");
            }
        }

        for (int i = 0; i < argv.size(); ++i) {
            std::string arg = argv[i];
            if (arg.starts_with("--")) {
                std::string argKey = arg.substr(2);
                if (mArguments.count(argKey)) {
                    auto& argument = mArguments[argKey];
                    if (argument.mIsFlag) {
                        argument.mValue = true;
                        argument.mSource = Source::CommandLine;
                    } else if (i + 1 < argv.size()) {
                        i++;

                        if (argument.mType == typeid(int)) {
                            argument.mValue = std::stoi(argv[i]);
                        } else if (argument.type == typeid(std::string)) {
                            argument.mValue = argv[i];
                        } else if (argument.type == typeid(double)) {
                            argument.mValue = std::stod(argv[i]);
                        } else if (argument.mType == typeid(bool)) {
                            argument.mValue = (argv[i] == "true" || argv[i] == "True" || argv[i] == "TRUE" || argv[i] == "1");
                        } else if (argument.mType == typeid(float)) {
                            argument.mValue = std::stof(argv[i]);
                        } else {
                            EURORA_THROW_EXCEPTION(ErrorCode::InvalidArgument, "Argument : " + argKey + " is not valid type!");
                        }

                        argument.source = Source::CommandLine;
                    }
                }
            }
        }
    }

    void ArgumentParser::parseConfigFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            EURORA_THROW_EXCEPTION(ErrorCode::OperationFailed, "Unable to open configuration file: " + filename);
        }
        nlohmann::json config;
        file >> config;

        for (const auto& [key, value] : config.items()) {
            if (mArguments.count(key)) {
                auto& argument = mArguments[key];
                if (argument.mConfigAllowed && argument.mSource != Source::CommandLine) {
                    if (argument.mType == typeid(int)) {
                        argument.mValue = value.get<int>();
                    } else if (argument.type == typeid(std::string)) {
                        argument.mValue = value.get<std::string>();
                    } else if (argument.type == typeid(double)) {
                        argument.mValue = value.get<double>();
                    } else if (argument.mType == typeid(bool)) {
                        argument.mValue = value.get<bool>();
                    } else if (argument.mType == typeid(float)) {
                        argument.mValue = value.get<float>();
                    } else {
                        EURORA_THROW_EXCEPTION(ErrorCode::InvalidArgument, "Argument : " + key + " is not valid type!");
                    }

                    argument.mSource = Source::ConfigFile;
                }
            }
        }
    }

    void ArgumentParser::validate() const {
        for (const auto& [name, arg] : mArguments) {
            if (arg.mRequired && arg.mSource == Source::None) {
                EURORA_THROW_EXCEPTION(ErrorCode::InvalidArgument, "Missing required argument: " + name);
            }
        }
    }

    void ArgumentParser::addHelpFlagKey(const std::string& helpFlagKey) {
        if (helpFlagKey.starts_with("-")) {
            mHelpFlagKeys.push_back(helpFlagKey);
        } else {
            if (helpFlagKey.length() > 1) {
                mHelpFlagKeys.push_back("--" + helpFlagKey);
            } else if (helpFlagKey.length() == 1) {
                mHelpFlagKeys.push_back("-" + helpFlagKey);
            }
        }
    }

    void ArgumentParser::help() const {
        STREAM_INFO() << "Available Options:\n";

        for (const auto& [name, arg] : mArguments) {
            std::stringstream info;
            info << "--" << std::setw(15) << std::left << name << " : ";
            info << (arg.mIsFlag ? "[flag]" : "[option]");

            if (arg.mRequired) {
                info << " [required]";
            } else {
                if (!arg.mIsFlag) {
                    info << " [default: ";

                    if (arg.mType == typeid(int)) {
                        info << std::get<int>(arg.mValue);
                    } else if (arg.mType == typeid(bool)) {
                        info << (std::get<bool>(arg.mValue) ? "true" : "false");
                    } else if (arg.mType == typeid(double)) {
                        info << std::get<double>(arg.mValue);
                    } else if (arg.mType == typeid(std::string)) {
                        info << std::get<std::string>(arg.mValue);
                    } else if (arg.mType == typeid(float)) {
                        info << std::get<float>(arg.mValue);
                    } else {
                        EURORA_THROW_EXCEPTION(ErrorCode::InvalidArgument, "Argument : " + name + " is not valid type!");
                    }

                    info << "]";

                    STREAM_INFO() << info.c_str();
                }
            }

            STREAM_INFO() << "\n " << arg.mDescription << "\n\n";
        }
    }

    void ArgumentParser::printParsedArguments() const {
        STREAM_INFO() << "Parsed Arguments:\n";

        for (const auto& [name, arg] : mArguments) {
            std::stringstream info;
            info << "--" << name << " : ";
            if (arg.mSource == Source::CommandLine) {
                info << "[CommandLine] ";
            } else if (arg.mSource == Source::ConfigFile) {
                info << "[ConfigFile] ";
            } else if (arg.mSource == Source::DefaultValue) {
                info << "[DefaultValue] ";
            } else {
                info << "[None] ";
            }

            if (arg.mType == typeid(int)) {
                info << std::get<int>(arg.mValue);
            } else if (arg.mType == typeid(float)) {
                info << std::get<float>(arg.mValue);
            } else if (arg.mType == typeid(double)) {
                info << std::get<double>(arg.mValue);
            } else if (arg.mType == typeid(bool)) {
                info << (std::get<bool>(arg.mValue) ? "true" : "false");
            } else if (arg.mType == typeid(std::string)) {
                info << std::get<std::string>(arg.mValue);
            }

            STREAM_INFO() << info.c_str();
        }
    }

} // namespace eurora::common
