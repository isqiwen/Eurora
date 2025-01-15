#pragma once

#include <vector>

#include "DynamicNDArray.h"

template <typename T>
class DynamicNDArrayView {
public:
    DynamicNDArrayView(const DynamicNDArray<T>& array, const std::vector<size_t>& start, const std::vector<size_t>& size)
        : parent_array_(array), start_(start), size_(size), strides_(calculateStrides(array.dimensions())) {}

    T& operator()(const std::vector<size_t>& indices) {
        auto global_indices = calculateGlobalIndices(indices);
        return parent_array_.operator()(global_indices);
    }

    const T& operator()(const std::vector<size_t>& indices) const {
        auto global_indices = calculateGlobalIndices(indices);
        return parent_array_.operator()(global_indices);
    }

private:
    const DynamicNDArray<T>& parent_array_;
    std::vector<size_t> start_;
    std::vector<size_t> size_;
    std::vector<size_t> strides_;

    std::vector<size_t> calculateStrides(const std::vector<size_t>& dimensions) const {
        std::vector<size_t> strides(dimensions.size());
        strides.back() = 1;
        for (int i = dimensions.size() - 2; i >= 0; --i) {
            strides[i] = strides[i + 1] * dimensions[i + 1];
        }
        return strides;
    }

    std::vector<size_t> calculateGlobalIndices(const std::vector<size_t>& indices) const {
        assert(indices.size() == size_.size());
        std::vector<size_t> global_indices(indices.size());
        for (size_t i = 0; i < indices.size(); ++i) {
            global_indices[i] = start_[i] + indices[i];
        }
        return global_indices;
    }
};
