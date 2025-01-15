#pragma once

#include <cmath>
#include <complex>
#include <iostream>
#include <type_traits>
#include <utility>

namespace eurora::core {

/**
     * \class complext
     * \brief A GPU- and CPU-compatible complex number class.
     */
template <typename T>
class complext {
public:
    // Default member initialization
    T _real = T(0);  // 实部
    T _imag = T(0);  // 虚部

    // Default constructor
    constexpr complext() noexcept = default;

    // Constructor with real and imaginary parts
    constexpr complext(T real, T imag) noexcept : _real(real), _imag(imag) {}

    // Implicit conversion from scalar
    constexpr explicit complext(T real) noexcept : _real(real), _imag(T(0)) {}

    // Implicit conversion from std::complex
    constexpr explicit complext(const std::complex<T>& cplx) noexcept : _real(cplx.real()), _imag(cplx.imag()) {}

    // Copy constructor (default)
    constexpr complext(const complext<T>& other) noexcept = default;

    // Move constructor (default)
    constexpr complext(complext<T>&& other) noexcept = default;

    // Assignment operators
    complext<T>& operator=(const complext<T>& other) noexcept = default;
    complext<T>& operator=(complext<T>&& other) noexcept      = default;

    // Accessors for real and imaginary parts
    constexpr T real() const noexcept { return _real; }

    constexpr T imag() const noexcept { return _imag; }

    // Conjugate
    constexpr complext<T> conj() const noexcept { return complext<T>(_real, -_imag); }

    // Magnitude (absolute value)
    constexpr T abs() const noexcept { return std::sqrt(_real * _real + _imag * _imag); }

    // Argument (phase angle)
    constexpr T arg() const noexcept { return std::atan2(_imag, _real); }

    // Norm (squared magnitude)
    constexpr T norm() const noexcept { return _real * _real + _imag * _imag; }

    // Polar form
    static constexpr complext<T> polar(T rho, T theta = T(0)) noexcept { return complext<T>(rho * std::cos(theta), rho * std::sin(theta)); }

    // Arithmetic operators
    constexpr complext<T>& operator+=(const complext<T>& other) noexcept {
        _real += other._real;
        _imag += other._imag;
        return *this;
    }

    constexpr complext<T>& operator-=(const complext<T>& other) noexcept {
        _real -= other._real;
        _imag -= other._imag;
        return *this;
    }

    constexpr complext<T>& operator*=(const complext<T>& other) noexcept {
        T real = _real * other._real - _imag * other._imag;
        T imag = _real * other._imag + _imag * other._real;
        _real  = real;
        _imag  = imag;
        return *this;
    }

    constexpr complext<T>& operator/=(const complext<T>& other) noexcept {
        T den  = other._real * other._real + other._imag * other._imag;
        T real = (_real * other._real + _imag * other._imag) / den;
        T imag = (_imag * other._real - _real * other._imag) / den;
        _real  = real;
        _imag  = imag;
        return *this;
    }

    // Unary negation
    constexpr complext<T> operator-() const noexcept { return complext<T>(-_real, -_imag); }
};

// Arithmetic operators
template <typename T>
constexpr complext<T> operator+(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    return complext<T>(lhs._real + rhs._real, lhs._imag + rhs._imag);
}

template <typename T>
constexpr complext<T> operator-(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    return complext<T>(lhs._real - rhs._real, lhs._imag - rhs._imag);
}

template <typename T>
constexpr complext<T> operator*(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    return complext<T>(lhs._real * rhs._real - lhs._imag * rhs._imag, lhs._real * rhs._imag + lhs._imag * rhs._real);
}

template <typename T>
constexpr complext<T> operator/(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    T den = rhs._real * rhs._real + rhs._imag * rhs._imag;
    return complext<T>((lhs._real * rhs._real + lhs._imag * rhs._imag) / den, (lhs._imag * rhs._real - lhs._real * rhs._imag) / den);
}

// Equality operators
template <typename T>
constexpr bool operator==(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    return lhs._real == rhs._real && lhs._imag == rhs._imag;
}

template <typename T>
constexpr bool operator!=(const complext<T>& lhs, const complext<T>& rhs) noexcept {
    return !(lhs == rhs);
}

// Output stream operator
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const complext<T>& cplx) {
    os << cplx.real() << (cplx.imag() >= 0 ? "+" : "") << cplx.imag() << "i";
    return os;
}

// Type aliases
using float_complext  = complext<float>;
using double_complext = complext<double>;

// Helper traits
template <typename T>
struct is_complex : std::false_type {};

template <typename T>
struct is_complex<complext<T>> : std::true_type {};

template <typename T>
constexpr bool is_complex_v = is_complex<T>::value;

}  // namespace eurora::core
