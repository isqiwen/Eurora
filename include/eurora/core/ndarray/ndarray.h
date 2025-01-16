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
    using element_type = T;

    NDArray()          = default;
    virtual ~NDArray() = default;

    // 基础创建和初始化
    virtual void Create(const std::vector<size_t>& dimensions)                                      = 0;
    virtual void Create(const std::vector<size_t>& dimensions, T* data, bool manage_memory = false) = 0;

    // 清理
    virtual void Clear() = 0;

    // 重设形状
    virtual void Reshape(const std::vector<size_t>& dimensions) = 0;
    virtual void Squeeze()                                      = 0;

    // 数据访问（多维索引、线性索引）
    virtual T& operator()(const std::vector<size_t>& indices)             = 0;
    virtual const T& operator()(const std::vector<size_t>& indices) const = 0;
    virtual T& operator[](size_t index)                                   = 0;  // 线性索引
    virtual const T& operator[](size_t index) const                       = 0;

    virtual T* Data()             = 0;
    virtual const T* Data() const = 0;

    // 尺寸和维度
    virtual size_t Size() const                           = 0;  // 总元素数
    virtual size_t Size(size_t dimension) const           = 0;  // 指定维度大小
    virtual const std::vector<size_t>& Dimensions() const = 0;

    // 偏移和索引计算
    virtual size_t CalculateOffset(const std::vector<size_t>& indices) const = 0;
    virtual std::vector<size_t> CalculateIndex(size_t offset) const          = 0;

    // 子数组和视图
    virtual std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const             = 0;
    virtual std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const                                      = 0;
    virtual std::shared_ptr<NDArray<T>> SliceView(const std::vector<size_t>& slice_start, const std::vector<size_t>& slice_end) const = 0;

    // 高级操作
    virtual void Apply(std::function<void(T&)> func)               = 0;  // 对每个元素应用函数
    virtual void Transform(const std::function<T(const T&)>& func) = 0;  // 对每个元素变换
    virtual void Fill(const T& value)                              = 0;  // 填充数组
    virtual std::shared_ptr<NDArray<T>> Copy() const               = 0;  // 深拷贝

    // 连续性检查
    virtual bool IsContiguous() const = 0;

    // 工具方法
    virtual bool PointInRange(const std::vector<size_t>& indices) const = 0;

protected:
    virtual void AllocateMemory()   = 0;
    virtual void DeallocateMemory() = 0;
};

}  // namespace eurora::core
