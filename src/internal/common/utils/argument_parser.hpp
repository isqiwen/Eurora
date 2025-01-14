#ifndef EURORA_COMMON_UTILS_ARGUMENT_PARSER_H_
#define EURORA_COMMON_UTILS_ARGUMENT_PARSER_H_

#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <variant>

#include "common/exception/exception.h"

namespace eurora::common {

EURORA_MAKE_SIMPLE_EXCEPTION(CallForHelp)

template <typename T>
struct is_valid_argument_type : std::disjunction<std::is_same<T, std::monostate>, std::is_same<T, bool>, std::is_same<T, int>, std::is_same<T, float>,
                                                 std::is_same<T, double>, std::is_same<T, std::string>> {};

template <typename T>
inline constexpr bool is_valid_argument_type_v = is_valid_argument_type<T>::value;

class ArgumentParser {
public:
  using ArgumentValue = std::variant<std::monostate, bool, int, float, double, std::string>;

  void AddFlag(const std::string& name, const std::string& description);

  template <typename T>
  void AddOption(const std::string& name, const std::string& description, T value = T(), bool config_allowed = true);

  template <typename T>
  void AddRequiredOption(const std::string& name, const std::string& description, bool config_allowed = true);

  template <typename T>
  T GetValueAs(const ArgumentValue& value) {
    return std::get<T>(value);
  }

  template <typename T>
  T Get(const std::string& name) const;

  void ParseCommandLine(const std::vector<std::string>& arguments);

  void ParseConfigFile(const std::string& filename);

  void Validate() const;

  void AddHelpFlagKey(const std::string& helpFlagKey);

  void Help() const;

  void PrintParsedArguments() const;

private:
  enum class Source { kNone, kDefaultValue, kCommandLine, kConfigFile };

  struct Argument {
    std::type_index type_ = typeid(void);
    std::string description_;
    bool required_ = false;
    bool is_flag_ = true;
    bool config_allowed_ = true;
    ArgumentValue value_;
    Source source_ = Source::kNone;
  };

  std::vector<std::string> help_flag_keys_ = {"-h", "--help"};
  std::unordered_map<std::string, Argument> arguments_;
};

template <typename T>
void ArgumentParser::AddOption(const std::string& name, const std::string& description, T value, bool config_allowed) {
  static_assert(is_valid_argument_type_v<T>, "T must be a valid type in bool, int, float, double, string");
  arguments_[name] = {typeid(T), description, false, false, config_allowed, value, Source::kDefaultValue};
}

template <typename T>
void ArgumentParser::AddRequiredOption(const std::string& name, const std::string& description, bool config_allowed) {
  static_assert(is_valid_argument_type_v<T>, "T must be a valid type in bool, int, float, double, string");
  arguments_[name] = {typeid(T), description, true, false, config_allowed, std::monostate{}, Source::kNone};
}

template <typename T>
T ArgumentParser::Get(const std::string& name) const {
  static_assert(is_valid_argument_type_v<T>, "T must be a valid argument type.");
  if (arguments_.count(name)) {
    return std::get<T>(arguments_[name].value);
  }

  EURORA_THROW_EXCEPTION(ErrorCode::kInvalidArgument, "Argument not found: " + name);
}

}  // namespace eurora::common

#endif  // EURORA_COMMON_UTILS_ARGUMENT_PARSER_H_
