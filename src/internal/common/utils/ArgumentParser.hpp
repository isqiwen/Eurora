#ifndef EURORA_COMMON_UTILS_ARGUMENT_PARSER_
#define EURORA_COMMON_UTILS_ARGUMENT_PARSER_

#pragma once

#include <unordered_map>
#include <string>
#include <variant>
#include <type_traits>
#include <iostream>
#include <stdexcept>
#include <typeindex>

#include "common/exception/Exception.h"

namespace eurora::common {

    EURORA_MAKE_SIMPLE_EXCEPTION(CallForHelp)

    template <typename T>
    struct is_valid_argument_type : std::disjunction<
        std::is_same<T, std::monostate>,
        std::is_same<T, bool>,
        std::is_same<T, int>,
        std::is_same<T, float>,
        std::is_same<T, double>,
        std::is_same<T, std::string>
    > {};

    template <typename T>
    inline constexpr bool is_valid_argument_type_v = is_valid_argument_type<T>::value;

    class ArgumentParser {
    public:
        using ArgumentValue = std::variant<std::monostate, bool, int, float, double, std::string>;

        void addFlag(const std::string& name, const std::string& description);

        template <typename T>
        void addOption(const std::string& name, const std::string& description, T value = T(), bool configAllowed = true);

        template <typename T>
        void addRequiredOption(const std::string& name, const std::string& description, bool configAllowed = true);

        template <typename T>
        T getValueAs(const ArgumentValue& value) {
            return std::get<T>(value);
        }

        template <typename T>
        T get(const std::string& name) const;

        void parseCommandLine(const std::vector<std::string>& arguments);

        void parseConfigFile(const std::string& filename);

        void validate() const;

        void addHelpFlagKey(const std::string& helpFlagKey);

        void help() const;

        void printParsedArguments() const;

    private:
        enum class Source { None, DefaultValue, CommandLine, ConfigFile };

        struct Argument {
            std::type_index             mType = typeid(void);
            std::string                 mDescription;
            bool                        mRequired = false;
            bool                        mIsFlag = true;
            bool                        mConfigAllowed = true;
            ArgumentValue               mValue;
            Source                      mSource = Source::None;
        };

        std::vector<std::string>        mHelpFlagKeys = {"-h", "--help"};
        std::unordered_map<std::string, Argument> mArguments;
    };

    template <typename T>
    void ArgumentParser::addOption(const std::string& name, const std::string& description, T value, bool configAllowed) {
        static_assert(is_valid_argument_type_v<T>, "T must be a valid type in bool, int, float, double, string");
        mArguments[name] = {typeid(T), description, false, false, configAllowed, value, Source::DefaultValue};
    }

    template <typename T>
    void ArgumentParser::addRequiredOption(const std::string& name, const std::string& description, bool configAllowed) {
        static_assert(is_valid_argument_type_v<T>, "T must be a valid type in bool, int, float, double, string");
        mArguments[name] = {typeid(T), description, true, false, configAllowed, std::monostate{}, Source::None};
    }

    template <typename T>
    T ArgumentParser::get(const std::string& name) const {
        static_assert(is_valid_argument_type_v<T>, "T must be a valid argument type.");
        if (mArguments.count(name)) {
            return std::get<T>(mArguments[name].value);
        }

        EURORA_THROW_EXCEPTION(ErrorCode::InvalidArgument, "Argument not found: " + name);
    }

} // namespace eurora::common

#endif // EURORA_COMMON_UTILS_ARGUMENT_PARSER_
