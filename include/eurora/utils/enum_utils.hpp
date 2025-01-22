#pragma once

#include <stdexcept>
#include <string>
#include <unordered_map>

namespace eurora::utils {

template <typename Enum>
struct EnumStrings {
    static const std::unordered_map<Enum, std::string>& Get();
};

template <typename Enum>
std::string ToString(Enum value) {
    const auto& map = EnumStrings<Enum>::Get();
    auto it         = map.find(value);
    if (it != map.end()) {
        return it->second;
    }
    throw std::invalid_argument("Invalid enum value");
}

template <typename Enum>
Enum FromString(const std::string& str) {
    const auto& map = EnumStrings<Enum>::Get();
    for (const auto& pair : map) {
        if (pair.second == str) {
            return pair.first;
        }
    }
    throw std::invalid_argument("Invalid enum string");
}

}  // namespace eurora::utils
