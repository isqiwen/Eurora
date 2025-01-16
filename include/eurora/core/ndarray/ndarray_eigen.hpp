#pragma once

#include <functional>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <unsupported/Eigen/CXX11/Tensor>
#include <vector>
#include "ndarray.h"

namespace eurora::core {

template <typename T>
class NDArrayEigen : public NDArray<T> {
public:
    using ValueType  = T;
    using TensorType = Eigen::TensorMap<Eigen::Tensor<T, Eigen::Dynamic>>;

    NDArrayEigen() = default;

    ~NDArrayEigen() override { Clear(); }

    void Create(const std::vector<size_t>& dimensions) override {
        if (dimensions.empty()) {
            throw std::runtime_error("Dimensions must not be empty.");
        }
        dimensions_ = dimensions;
        elements_   = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        AllocateMemory();
        UpdateTensorMap();
    }

    void Create(const std::vector<size_t>& dimensions, T* data, bool manage_memory = false) override {
        if (!data) {
            throw std::runtime_error("Data pointer must not be null.");
        }
        dimensions_    = dimensions;
        elements_      = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        data_          = data;
        manage_memory_ = manage_memory;
        UpdateTensorMap();
    }

    void Clear() override {
        if (manage_memory_ && data_) {
            DeallocateMemory();
        }
        data_ = nullptr;
        dimensions_.clear();
        elements_ = 0;
    }

    void Reshape(const std::vector<size_t>& dimensions) override {
        size_t new_size = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        if (new_size != elements_) {
            throw std::runtime_error("Reshape must not change the total number of elements.");
        }
        dimensions_ = dimensions;
        UpdateTensorMap();
    }

    void Squeeze() override {
        dimensions_.erase(std::remove(dimensions_.begin(), dimensions_.end(), 1), dimensions_.end());
        UpdateTensorMap();
    }

    T& operator()(const std::vector<size_t>& indices) override {
        size_t offset = CalculateOffset(indices);
        return data_[offset];
    }

    const T& operator()(const std::vector<size_t>& indices) const override {
        size_t offset = CalculateOffset(indices);
        return data_[offset];
    }

    T& operator[](size_t index) override {
        if (index >= elements_) {
            throw std::runtime_error("Index out of range.");
        }
        return data_[index];
    }

    const T& operator[](size_t index) const override {
        if (index >= elements_) {
            throw std::runtime_error("Index out of range.");
        }
        return data_[index];
    }

    T* Data() override { return data_; }

    const T* Data() const override { return data_; }

    size_t Size() const override { return elements_; }

    size_t Size(size_t dimension) const override {
        if (dimension >= dimensions_.size()) {
            throw std::runtime_error("Dimension out of range.");
        }
        return dimensions_[dimension];
    }

    const std::vector<size_t>& Dimensions() const override { return dimensions_; }

    size_t CalculateOffset(const std::vector<size_t>& indices) const override {
        if (indices.size() != dimensions_.size()) {
            throw std::runtime_error("Indices must match the number of dimensions.");
        }
        return std::inner_product(indices.begin(), indices.end(), offset_factors_.begin(), size_t(0));
    }

    std::vector<size_t> CalculateIndex(size_t offset) const override {
        if (offset >= elements_) {
            throw std::runtime_error("Offset out of range.");
        }
        std::vector<size_t> indices(dimensions_.size(), 0);
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            indices[i] = offset / offset_factors_[i];
            offset %= offset_factors_[i];
        }
        return indices;
    }

    std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const override {
        if (start.size() != dimensions_.size() || size.size() != dimensions_.size()) {
            throw std::runtime_error("Dimension mismatch.");
        }
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (start[i] + size[i] > dimensions_[i]) {
                throw std::runtime_error("Subarray out of bounds.");
            }
        }

        auto view            = std::make_shared<NDArrayEigen<T>>();
        view->dimensions_    = size;
        view->elements_      = std::accumulate(size.begin(), size.end(), size_t(1), std::multiplies<>());
        view->data_          = data_ + CalculateOffset(start);
        view->manage_memory_ = false;
        view->UpdateTensorMap();
        return view;
    }

    std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const override {
        size_t new_elements = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        if (new_elements != elements_) {
            throw std::runtime_error("Element count mismatch.");
        }

        auto view            = std::make_shared<NDArrayEigen<T>>();
        view->dimensions_    = dimensions;
        view->elements_      = elements_;
        view->data_          = data_;
        view->manage_memory_ = false;
        view->UpdateTensorMap();
        return view;
    }

    std::shared_ptr<NDArray<T>> SliceView(const std::vector<size_t>& slice_start, const std::vector<size_t>& slice_end) const override {
        if (slice_start.size() != dimensions_.size() || slice_end.size() != dimensions_.size()) {
            throw std::runtime_error("NDArrayEigen::SliceView: Dimension mismatch");
        }

        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (slice_start[i] >= slice_end[i] || slice_end[i] > dimensions_[i]) {
                throw std::runtime_error("NDArrayEigen::SliceView: Invalid slice range");
            }
        }

        auto view = std::make_shared<NDArrayEigen<T>>();
        view->dimensions_.resize(dimensions_.size());
        view->size_ = 1;
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            view->dimensions_[i] = slice_end[i] - slice_start[i];
            view->size_ *= view->dimensions_[i];
        }
        view->data_          = data_ + CalculateOffset(slice_start);
        view->manage_memory_ = false;
        view->UpdateTensorMap();
        return view;
    }

    void Apply(std::function<void(T&)> func) override {
        for (size_t i = 0; i < elements_; ++i) {
            func(data_[i]);
        }
    }

    void Transform(const std::function<T(const T&)>& func) override {
        for (size_t i = 0; i < elements_; ++i) {
            data_[i] = func(data_[i]);
        }
    }

    void Fill(const T& value) override {
        for (size_t i = 0; i < elements_; ++i) {
            data_[i] = value;
        }
    }

    std::shared_ptr<NDArray<T>> Copy() const override {
        auto copy = std::make_shared<NDArrayEigen<T>>();
        copy->Create(dimensions_);
        std::copy(data_, data_ + elements_, copy->data_);
        return copy;
    }

    bool IsContiguous() const override { return true; }

    bool PointInRange(const std::vector<size_t>& indices) const override {
        if (indices.size() != dimensions_.size())
            return false;
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (indices[i] >= dimensions_[i])
                return false;
        }
        return true;
    }

private:
    void AllocateMemory() override {
        data_          = new T[elements_];
        manage_memory_ = true;
        UpdateOffsetFactors();
    }

    void DeallocateMemory() override {
        delete[] data_;
        data_ = nullptr;
    }

    void UpdateTensorMap() {
        Eigen::array<Eigen::Index, Eigen::Dynamic> dims(dimensions_.size());
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            dims[i] = static_cast<Eigen::Index>(dimensions_[i]);
        }
        tensor_ = TensorType(data_, dims);
    }

    void UpdateOffsetFactors() {
        offset_factors_.resize(dimensions_.size());
        size_t factor = 1;
        for (size_t i = dimensions_.size(); i > 0; --i) {
            offset_factors_[i - 1] = factor;
            factor *= dimensions_[i - 1];
        }
    }

    std::vector<size_t> dimensions_;
    std::vector<size_t> offset_factors_;
    std::vector<size_t> strides_;
    size_t elements_    = 0;
    T* data_            = nullptr;
    bool manage_memory_ = false;
    bool is_contiguous_ = true;
    TensorType tensor_;
};

}  // namespace eurora::core
