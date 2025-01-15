#pragma once

#include <Eigen/CXX11/Tensor>
#include <Eigen/Dense>
#include <cassert>
#include <numeric>
#include <vector>

template <typename T>
class DynamicNDArray {
public:
    DynamicNDArray() = default;

    explicit DynamicNDArray(const std::vector<size_t>& dimensions) : dimensions_(dimensions), tensor_(calculateTensorDimensions(dimensions)) {}

    void resize(const std::vector<size_t>& dimensions) {
        dimensions_ = dimensions;
        tensor_     = Eigen::Tensor<T, Eigen::Dynamic>(calculateTensorDimensions(dimensions));
    }

    T& operator()(const std::vector<size_t>& indices) {
        assert(indices.size() == dimensions_.size());
        return tensor_(calculateOffset(indices));
    }

    const T& operator()(const std::vector<size_t>& indices) const {
        assert(indices.size() == dimensions_.size());
        return tensor_(calculateOffset(indices));
    }

    size_t size() const { return tensor_.size(); }

    const std::vector<size_t>& dimensions() const { return dimensions_; }

private:
    std::vector<size_t> dimensions_;
    Eigen::Tensor<T, Eigen::Dynamic> tensor_;

    Eigen::array<Eigen::Index, Eigen::Dynamic> calculateTensorDimensions(const std::vector<size_t>& dimensions) {
        Eigen::array<Eigen::Index, Eigen::Dynamic> tensor_dims(dimensions.size());
        for (size_t i = 0; i < dimensions.size(); ++i) {
            tensor_dims[i] = static_cast<Eigen::Index>(dimensions[i]);
        }
        return tensor_dims;
    }

    size_t calculateOffset(const std::vector<size_t>& indices) const {
        size_t offset = 0;
        size_t stride = 1;
        for (int i = dimensions_.size() - 1; i >= 0; --i) {
            offset += indices[i] * stride;
            stride *= dimensions_[i];
        }
        return offset;
    }
};
