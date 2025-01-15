#pragma once

#include <array>
#include <cmath>
#include <iostream>
#include <numeric>
#include <stdexcept>

namespace eurora::core {

template <typename T, size_t D>
class NDPoint {
public:
    // 类型别名
    using value_type = T;
    using Self       = NDPoint<T, D>;

    // 默认构造函数
    constexpr NDPoint() noexcept : data_{} {}

    // 用标量填充点的所有维度
    explicit constexpr NDPoint(const T& value) noexcept { data_.fill(value); }

    // 复制构造
    constexpr NDPoint(const Self& other) noexcept = default;

    // 拷贝赋值
    constexpr Self& operator=(const Self& other) noexcept = default;

    // 元素访问
    constexpr T& operator[](size_t idx) {
        if (idx >= D)
            throw std::out_of_range("Index out of range");
        return data_[idx];
    }

    constexpr const T& operator[](size_t idx) const {
        if (idx >= D)
            throw std::out_of_range("Index out of range");
        return data_[idx];
    }

    // 填充整个点
    void fill(const T& value) { data_.fill(value); }

    // 算术操作
    constexpr Self& operator+=(const Self& other) {
        for (size_t i = 0; i < D; ++i)
            data_[i] += other.data_[i];
        return *this;
    }

    constexpr Self& operator-=(const Self& other) {
        for (size_t i = 0; i < D; ++i)
            data_[i] -= other.data_[i];
        return *this;
    }

    constexpr Self& operator*=(const Self& other) {
        for (size_t i = 0; i < D; ++i)
            data_[i] *= other.data_[i];
        return *this;
    }

    constexpr Self& operator/=(const Self& other) {
        for (size_t i = 0; i < D; ++i) {
            if (std::abs(other.data_[i]) < std::numeric_limits<T>::epsilon())
                throw std::overflow_error("Division by zero");
            data_[i] /= other.data_[i];
        }
        return *this;
    }

    // 标量算术操作
    constexpr Self& operator+=(const T& scalar) {
        for (auto& elem : data_)
            elem += scalar;
        return *this;
    }

    constexpr Self& operator-=(const T& scalar) {
        for (auto& elem : data_)
            elem -= scalar;
        return *this;
    }

    constexpr Self& operator*=(const T& scalar) {
        for (auto& elem : data_)
            elem *= scalar;
        return *this;
    }

    constexpr Self& operator/=(const T& scalar) {
        if (std::abs(scalar) < std::numeric_limits<T>::epsilon())
            throw std::overflow_error("Division by zero");
        for (auto& elem : data_)
            elem /= scalar;
        return *this;
    }

    // 点积
    constexpr T dot(const Self& other) const { return std::inner_product(data_.begin(), data_.end(), other.data_.begin(), T(0)); }

    // 模长
    constexpr T abs() const { return std::sqrt(this->dot(*this)); }

    // 归一化
    void normalize() {
        T magnitude = abs();
        if (magnitude < std::numeric_limits<T>::epsilon())
            return;
        for (auto& elem : data_)
            elem /= magnitude;
    }

    // 输出
    void print(std::ostream& os) const {
        os << "[";
        for (size_t i = 0; i < D - 1; ++i)
            os << data_[i] << ", ";
        os << data_[D - 1] << "]";
    }

private:
    std::array<T, D> data_;  // 使用 std::array 存储点数据
};

// 流输出运算符
template <typename T, size_t D>
std::ostream& operator<<(std::ostream& os, const NDPoint<T, D>& point) {
    point.print(os);
    return os;
}

}  // namespace eurora::core
