#pragma once

#include <algorithm>
#include <memory>
#include <numeric>
#include <stdexcept>
#include <vector>

#include "ndarray.h"

namespace eurora::core {

template <typename T>
class NDArrayView : public NDArray<T> {
public:
    // 构造函数：传入底层 NDArray 的 shared_ptr、视图的维度、视图的步长以及起始偏移（相对于底层数据）
    NDArrayView(std::shared_ptr<NDArray<T>> base, std::vector<size_t> dimensions, std::vector<size_t> strides, size_t offset = 0)
        : base_{std::move(base)}, dimensions_{std::move(dimensions)}, strides_{std::move(strides)}, offset_{offset} {
        if (dimensions_.empty() || strides_.empty() || dimensions_.size() != strides_.size())
            throw std::runtime_error("Invalid dimensions or strides in NDArrayView constructor.");
        elements_ = std::accumulate(dimensions_.begin(), dimensions_.end(), size_t{1}, std::multiplies<>());
    }

    ~NDArrayView() override = default;

    // 对于视图，不支持创建新内存
    void Create(const std::vector<size_t>&) override { throw std::runtime_error("NDArrayView does not support Create."); }

    void Create(const std::vector<size_t>&, T*, bool) override { throw std::runtime_error("NDArrayView does not support Create."); }

    // Clear 仅清空视图信息，不影响底层数据
    void Clear() override {
        dimensions_.clear();
        strides_.clear();
        elements_ = 0;
        offset_   = 0;
        base_.reset();
    }

    // 重新定义视图形状（仅允许在元素数量不变时操作）
    void Reshape(const std::vector<size_t>& dimensions) override {
        size_t new_elements = std::accumulate(dimensions.begin(), dimensions.end(), size_t{1}, std::multiplies<>());
        if (new_elements != elements_)
            throw std::runtime_error("Reshape must not change total number of elements.");
        dimensions_ = dimensions;
        // 对视图假定采用行优先连续布局，从而生成新的连续步长
        strides_.resize(dimensions_.size());
        size_t factor = 1;
        for (size_t i = dimensions_.size(); i > 0; --i) {
            strides_[i - 1] = factor;
            factor *= dimensions_[i - 1];
        }
    }

    // Squeeze 去除尺寸为1的维度，同时更新步长
    void Squeeze() override {
        std::vector<size_t> new_dims;
        std::vector<size_t> new_strides;
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (dimensions_[i] != 1) {
                new_dims.push_back(dimensions_[i]);
                new_strides.push_back(strides_[i]);
            }
        }
        dimensions_ = std::move(new_dims);
        strides_    = std::move(new_strides);
    }

    // 根据视图内的多维下标计算在底层数组中的绝对偏移
    size_t CalculateOffset(const std::vector<size_t>& indices) const override {
        if (indices.size() != dimensions_.size())
            throw std::runtime_error("Indices size mismatch in NDArrayView::CalculateOffset.");
        for (size_t i = 0; i < indices.size(); ++i)
            if (indices[i] >= dimensions_[i])
                throw std::runtime_error("Index out of range in NDArrayView::CalculateOffset.");
        size_t rel_offset = std::inner_product(indices.begin(), indices.end(), strides_.begin(), size_t{0});
        return offset_ + rel_offset;
    }

    // 将视图线性下标转换为多维下标（假定视图数据是按连续布局排列的）
    std::vector<size_t> CalculateIndex(size_t linear_index) const override {
        if (linear_index >= elements_)
            throw std::runtime_error("Linear index out of range in NDArrayView::CalculateIndex.");
        // 计算“连续布局”的步长因子（row-major）
        std::vector<size_t> factors(dimensions_.size());
        size_t factor = 1;
        for (size_t i = dimensions_.size(); i > 0; --i) {
            factors[i - 1] = factor;
            factor *= dimensions_[i - 1];
        }
        std::vector<size_t> indices(dimensions_.size(), 0);
        size_t remaining = linear_index;
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            indices[i] = remaining / factors[i];
            remaining %= factors[i];
        }
        return indices;
    }

    T& operator()(const std::vector<size_t>& indices) override {
        size_t off = CalculateOffset(indices);
        return base_->Data()[off];
    }

    const T& operator()(const std::vector<size_t>& indices) const override {
        size_t off = CalculateOffset(indices);
        return base_->Data()[off];
    }

    T& operator[](size_t index) override {
        if (index >= elements_)
            throw std::runtime_error("Index out of range in NDArrayView::operator[].");
        std::vector<size_t> indices = CalculateIndex(index);
        return (*this)(indices);
    }

    const T& operator[](size_t index) const override {
        if (index >= elements_)
            throw std::runtime_error("Index out of range in NDArrayView::operator[].");
        std::vector<size_t> indices = CalculateIndex(index);
        return (*this)(indices);
    }

    T* Data() override { return base_->Data() + offset_; }

    const T* Data() const override { return base_->Data() + offset_; }

    size_t Size() const override { return elements_; }

    size_t Size(size_t dimension) const override {
        if (dimension >= dimensions_.size())
            throw std::runtime_error("Dimension out of range in NDArrayView::Size(dimension).");
        return dimensions_[dimension];
    }

    const std::vector<size_t>& Dimensions() const override { return dimensions_; }

    // 生成子视图（视图的子范围），依然共享底层数据
    std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const override {
        if (start.size() != dimensions_.size() || size.size() != dimensions_.size())
            throw std::runtime_error("Dimension mismatch in NDArrayView::Subarray.");
        for (size_t i = 0; i < dimensions_.size(); ++i)
            if (start[i] + size[i] > dimensions_[i])
                throw std::runtime_error("Subarray out of range in NDArrayView::Subarray.");
        size_t new_offset = offset_ + std::inner_product(start.begin(), start.end(), strides_.begin(), size_t{0});
        return std::make_shared<NDArrayView<T>>(base_, size, strides_, new_offset);
    }

    // 重塑视图：生成一个新视图，假定数据在视图内是连续的
    std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const override {
        size_t new_elements = std::accumulate(dimensions.begin(), dimensions.end(), size_t{1}, std::multiplies<>());
        if (new_elements != elements_)
            throw std::runtime_error("Element count mismatch in NDArrayView::ReshapeView.");
        std::vector<size_t> new_strides(dimensions.size());
        size_t factor = 1;
        for (size_t i = dimensions.size(); i > 0; --i) {
            new_strides[i - 1] = factor;
            factor *= dimensions[i - 1];
        }
        return std::make_shared<NDArrayView<T>>(base_, dimensions, new_strides, offset_);
    }

    // 切片视图：以 slice_start 和 slice_end 定义新视图
    std::shared_ptr<NDArray<T>> SliceView(const std::vector<size_t>& slice_start, const std::vector<size_t>& slice_end) const override {
        if (slice_start.size() != dimensions_.size() || slice_end.size() != dimensions_.size())
            throw std::runtime_error("Dimension mismatch in NDArrayView::SliceView.");
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (slice_start[i] >= slice_end[i] || slice_end[i] > dimensions_[i])
                throw std::runtime_error("Invalid slice range in NDArrayView::SliceView.");
        }
        std::vector<size_t> new_dims(dimensions_.size());
        for (size_t i = 0; i < dimensions_.size(); ++i)
            new_dims[i] = slice_end[i] - slice_start[i];
        size_t new_offset = offset_ + std::inner_product(slice_start.begin(), slice_start.end(), strides_.begin(), size_t{0});
        return std::make_shared<NDArrayView<T>>(base_, new_dims, strides_, new_offset);
    }

    void Apply(std::function<void(T&)> func) override {
        for (size_t i = 0; i < elements_; ++i)
            (*this)[i] = func((*this)[i]);
    }

    void Transform(const std::function<T(const T&)>& func) override {
        for (size_t i = 0; i < elements_; ++i)
            (*this)[i] = func((*this)[i]);
    }

    void Fill(const T& value) override {
        for (size_t i = 0; i < elements_; ++i)
            (*this)[i] = value;
    }

    // 复制生成新数组（复制的数据连续存储，新数组为 NDArrayEigen 实现）
    std::shared_ptr<NDArray<T>> Copy() const override {
        auto new_array = std::make_shared<NDArrayEigen<T>>();
        new_array->Create(dimensions_);
        for (size_t i = 0; i < elements_; ++i)
            (*new_array)[i] = (*this)[i];
        return new_array;
    }

    bool IsContiguous() const override {
        // 认为当 strides_ 等于连续存储时计算出的步长时视为连续
        std::vector<size_t> contiguous_strides(dimensions_.size());
        size_t factor = 1;
        for (size_t i = dimensions_.size(); i > 0; --i) {
            contiguous_strides[i - 1] = factor;
            factor *= dimensions_[i - 1];
        }
        return strides_ == contiguous_strides;
    }

    bool PointInRange(const std::vector<size_t>& indices) const override {
        if (indices.size() != dimensions_.size())
            return false;
        for (size_t i = 0; i < dimensions_.size(); ++i)
            if (indices[i] >= dimensions_[i])
                return false;
        return true;
    }

private:
    std::shared_ptr<NDArray<T>> base_;  // 对底层数组的引用
    std::vector<size_t> dimensions_;    // 视图维度
    std::vector<size_t> strides_;       // 视图步长（相对于底层数据）
    size_t offset_   = 0;               // 在底层数据中的起始偏移
    size_t elements_ = 0;               // 视图元素总数
};

}  // namespace eurora::core
