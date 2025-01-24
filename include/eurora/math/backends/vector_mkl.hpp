#pragma once

#include <mkl.h>

#include "eurora/math/types.h"

namespace eurora::math {

template <typename T>
struct VectorMKLBackend {
    static Vector<T> Add(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());

        if constexpr (std::is_same_v<T, float>) {
            cblas_scopy(a.size(), a.data(), 1, result.data(), 1); // result = a
            cblas_saxpy(a.size(), 1.0f, b.data(), 1, result.data(), 1); // result += b
        } else if constexpr (std::is_same_v<T, double>) {
            cblas_dcopy(a.size(), a.data(), 1, result.data(), 1); // result = a
            cblas_daxpy(a.size(), 1.0, b.data(), 1, result.data(), 1); // result += b
        } else {
            static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                          "MKL only supports float and double");
        }

        return result;
    }

    static Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());

        if constexpr (std::is_same_v<T, float>) {
            cblas_scopy(a.size(), a.data(), 1, result.data(), 1); // result = a
            cblas_saxpy(a.size(), -1.0f, b.data(), 1, result.data(), 1); // result -= b
        } else if constexpr (std::is_same_v<T, double>) {
            cblas_dcopy(a.size(), a.data(), 1, result.data(), 1); // result = a
            cblas_daxpy(a.size(), -1.0, b.data(), 1, result.data(), 1); // result -= b
        } else {
            static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>,
                          "MKL only supports float and double");
        }

        for (int i = 0; i < a.size(); ++i)
        {
            result[i] = a[i] - b[i];
        }

        return result;
    }
};

}
