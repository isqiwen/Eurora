#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace eurora::core {

template <typename T>
class NDArray {
public:
    using element_type = T;

    NDArray()          = default;
    virtual ~NDArray() = default;

    virtual void Create(const std::vector<size_t>& dimensions)                                      = 0;
    virtual void Create(const std::vector<size_t>& dimensions, T* data, bool manage_memory = false) = 0;

    virtual void Clear();

    virtual void Reshape(const std::vector<size_t>& dimensions) = 0;
    virtual void Squeeze()                                      = 0;

    virtual T& operator[](size_t index)             = 0;
    virtual const T& operator[](size_t index) const = 0;

    virtual T* Data()             = 0;
    virtual const T* Data() const = 0;

    virtual size_t Size() const                           = 0;
    virtual size_t Size(size_t dimension) const           = 0;
    virtual const std::vector<size_t>& Dimensions() const = 0;

    virtual size_t CalculateOffset(const std::vector<size_t>& indices) const = 0;

    virtual std::vector<size_t> CalculateIndex(size_t offset) const = 0;

    virtual std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const = 0;
    virtual std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const                          = 0;

    template <typename... Indices>
    size_t CalculateOffset(Indices... indices) const {
        constexpr size_t num_indices = sizeof...(indices);
        if (num_indices != Dimensions().size()) {
            throw std::runtime_error("NDArrayEigen::CalculateOffset: Dimension mismatch.");
        }
        std::array<size_t, num_indices> indices_array = {static_cast<size_t>(indices)...};
        return CalculateOffset(std::vector<size_t>(indices_array.begin(), indices_array.end()));
    }
};
}  // namespace eurora::core
