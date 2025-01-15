#pragma once

#include <initializer_list>
#include <iostream>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "core/complext.hpp"
#include "core/ndarray.hpp"

namespace eurora::core {

namespace Indexing {
struct Slice {};

constexpr auto slice = Slice{};
}  // namespace Indexing

template <typename T>
class CpuNDArray : public NDArray<T> {
public:
    using BaseClass = NDArray<T>;
    using ValueType = T;

    // Constructors
    CpuNDArray() = default;

    explicit CpuNDArray(const std::vector<size_t>& dimensions) : dimensions_(dimensions) { allocate_memory(); }

    CpuNDArray(const std::vector<size_t>& dimensions, T* data, bool manage_memory = true)
        : dimensions_(dimensions), data_(data), manage_memory_(manage_memory) {
        calculate_offset_factors();
    }

    CpuNDArray(std::initializer_list<size_t> dimensions) : CpuNDArray(std::vector<size_t>(dimensions)) {}

    CpuNDArray(size_t len) : CpuNDArray(std::vector<size_t>{len}) {}

    template <typename... Sizes, typename = std::enable_if_t<(std::is_integral_v<Sizes> && ...)>>
    CpuNDArray(Sizes... sizes) : CpuNDArray(std::vector<size_t>{static_cast<size_t>(sizes)...}) {}

    CpuNDArray(const CpuNDArray& other) { copy_from(other); }

    CpuNDArray(CpuNDArray&& other) noexcept : dimensions_(std::move(other.dimensions_)), data_(other.data_), manage_memory_(other.manage_memory_) {
        other.data_          = nullptr;
        other.manage_memory_ = false;
    }

    ~CpuNDArray() {
        if (manage_memory_ && data_) {
            delete[] data_;
        }
    }

    // Assignment operators
    CpuNDArray& operator=(const CpuNDArray& other) {
        if (this != &other) {
            copy_from(other);
        }
        return *this;
    }

    CpuNDArray& operator=(CpuNDArray&& other) noexcept {
        if (this != &other) {
            if (manage_memory_ && data_) {
                delete[] data_;
            }
            dimensions_    = std::move(other.dimensions_);
            data_          = other.data_;
            manage_memory_ = other.manage_memory_;

            other.data_          = nullptr;
            other.manage_memory_ = false;
        }
        return *this;
    }

    // Element access
    T& operator()(size_t x) { return data_[x]; }

    template <typename... Indices, typename = std::enable_if_t<(std::is_integral_v<Indices> && ...)>>
    T& operator()(Indices... indices) {
        static_assert(sizeof...(Indices) == dimensions_.size(), "Incorrect number of indices.");
        size_t offset = calculate_offset({static_cast<size_t>(indices)...});
        return data_[offset];
    }

    const T& operator()(size_t x) const { return data_[x]; }

    template <typename... Indices, typename = std::enable_if_t<(std::is_integral_v<Indices> && ...)>>
    const T& operator()(Indices... indices) const {
        static_assert(sizeof...(Indices) == dimensions_.size(), "Incorrect number of indices.");
        size_t offset = calculate_offset({static_cast<size_t>(indices)...});
        return data_[offset];
    }

    // Utility methods
    void fill(const T& value) { std::fill(data_, data_ + size(), value); }

    size_t size() const { return std::accumulate(dimensions_.begin(), dimensions_.end(), size_t(1), std::multiplies<>()); }

    const std::vector<size_t>& dimensions() const { return dimensions_; }

    void reshape(const std::vector<size_t>& new_dims) {
        if (std::accumulate(new_dims.begin(), new_dims.end(), size_t(1), std::multiplies<>()) != size()) {
            throw std::runtime_error("New dimensions must have the same total size.");
        }
        dimensions_ = new_dims;
        calculate_offset_factors();
    }

    // Iterators
    T* begin() { return data_; }

    T* end() { return data_ + size(); }

    const T* begin() const { return data_; }

    const T* end() const { return data_ + size(); }

    // Debugging
    void print(std::ostream& os) const {
        os << "Dimensions: [";
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            os << dimensions_[i] << (i + 1 < dimensions_.size() ? ", " : "");
        }
        os << "]\nData: ";
        for (const auto& val : *this) {
            os << val << " ";
        }
        os << "\n";
    }

private:
    std::vector<size_t> dimensions_;
    std::vector<size_t> offset_factors_;
    T* data_            = nullptr;
    bool manage_memory_ = true;

    void allocate_memory() {
        if (data_) {
            throw std::runtime_error("Memory already allocated.");
        }
        data_ = new T[size()];
        calculate_offset_factors();
    }

    void calculate_offset_factors() {
        offset_factors_.resize(dimensions_.size());
        size_t factor = 1;
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            offset_factors_[i] = factor;
            factor *= dimensions_[i];
        }
    }

    size_t calculate_offset(const std::vector<size_t>& indices) const {
        size_t offset = 0;
        for (size_t i = 0; i < indices.size(); ++i) {
            offset += indices[i] * offset_factors_[i];
        }
        return offset;
    }

    void copy_from(const CpuNDArray& other) {
        dimensions_ = other.dimensions_;
        allocate_memory();
        std::copy(other.begin(), other.end(), begin());
    }
};

// Debug operator
template <typename T>
std::ostream& operator<<(std::ostream& os, const CpuNDArray<T>& array) {
    array.print(os);
    return os;
}

}  // namespace eurora::core
