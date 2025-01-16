#ifndef EURORA_UTILS_STRING_UTILS_H_
#define EURORA_UTILS_STRING_UTILS_H_

#pragma once

#include <list>
#include <numeric>
#include <string>
#include <vector>

namespace eurora::utils {

class StringUtils {
public:
    static bool StartsWith(const std::string& source_str, const std::string& prefix);
};

}  // namespace eurora::utils

#endif  // EURORA_UTILS_STRING_UTILS_H_
