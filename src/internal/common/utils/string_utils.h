#ifndef EURORA_COMMON_UTILS_STRING_UTILS_H_
#define EURORA_COMMON_UTILS_STRING_UTILS_H_

#pragma once

#include <list>
#include <numeric>
#include <string>
#include <vector>

namespace eurora::common {

class StringUtils {
public:
    static bool StartsWith(const std::string& source_str, const std::string& prefix);
};

}  // namespace eurora::common

#endif  // EURORA_COMMON_UTILS_STRING_UTILS_H_
