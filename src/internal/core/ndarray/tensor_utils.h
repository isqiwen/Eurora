#pragma once

#include <numeric>
#include <vector>

namespace TensorUtils {

inline std::vector<size_t> calculateStrides(const std::vector<size_t>& dimensions) {
    std::vector<size_t> strides(dimensions.size());
    strides.back() = 1;
    for (int i = dimensions.size() - 2; i >= 0; --i) {
        strides[i] = strides[i + 1] * dimensions[i + 1];
    }
    return strides;
}

inline size_t calculateOffset(const std::vector<size_t>& indices, const std::vector<size_t>& strides) {
    size_t offset = 0;
    for (size_t i = 0; i < indices.size(); ++i) {
        offset += indices[i] * strides[i];
    }
    return offset;
}

}  // namespace TensorUtils
