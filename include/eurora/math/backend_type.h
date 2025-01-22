#pragma once

#include "eurora/utils/enum_utils.hpp"

namespace eurora::math {

enum class BackendType : size_t { MKL = 0, Eigen, Armadillo, NumCpp, Count };

}  // namespace eurora::math

namespace eurora::utils {

template <>
struct EnumStrings<eurora::math::BackendType> {
    static const std::unordered_map<eurora::math::BackendType, std::string>& Get() {
        static const std::unordered_map<eurora::math::BackendType, std::string> mapping = {{eurora::math::BackendType::MKL, "MKL"},
                                                                                           {eurora::math::BackendType::Eigen, "Eigen"},
                                                                                           {eurora::math::BackendType::Armadillo, "Armadillo"},
                                                                                           {eurora::math::BackendType::NumCpp, "NumCpp"},
                                                                                           {eurora::math::BackendType::Count, "Count"}};
        return mapping;
    }
};

}  // namespace eurora::utils
