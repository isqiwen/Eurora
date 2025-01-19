#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace eurora::core {

template <typename T>
class NDArray {
public:
    using ValueType = T;

    NDArray()          = default;
    virtual ~NDArray() = default;

    virtual void Create(const std::vector<size_t>& dimensions)                                      = 0;
    virtual void Create(const std::vector<size_t>& dimensions, T* data, bool manage_memory = false) = 0;
    virtual void Clear()                                                                            = 0;

    virtual void Reshape(const std::vector<size_t>& dimensions) = 0;
    virtual void Squeeze()                                      = 0;

    virtual T& operator()(const std::vector<size_t>& indices)             = 0;
    virtual const T& operator()(const std::vector<size_t>& indices) const = 0;
    virtual T& operator[](size_t index)                                   = 0;
    virtual const T& operator[](size_t index) const                       = 0;
    virtual T* Data()                                                     = 0;
    virtual const T* Data() const                                         = 0;
    virtual size_t Size() const                                           = 0;
    virtual size_t Size(size_t dimension) const                           = 0;
    virtual const std::vector<size_t>& Dimensions() const                 = 0;

    virtual size_t CalculateOffset(const std::vector<size_t>& indices) const = 0;
    virtual std::vector<size_t> CalculateIndex(size_t offset) const          = 0;

    virtual std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const             = 0;
    virtual std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const                                      = 0;
    virtual std::shared_ptr<NDArray<T>> SliceView(const std::vector<size_t>& slice_start, const std::vector<size_t>& slice_end) const = 0;

    virtual void Apply(std::function<void(T&)> func)               = 0;
    virtual void Transform(const std::function<T(const T&)>& func) = 0;
    virtual void Fill(const T& value)                              = 0;
    virtual std::shared_ptr<NDArray<T>> Copy() const               = 0;

    virtual bool IsContiguous() const = 0;

    virtual bool PointInRange(const std::vector<size_t>& indices) const = 0;

protected:
    virtual void AllocateMemory()   = 0;
    virtual void DeallocateMemory() = 0;
};

}  // namespace eurora::core
