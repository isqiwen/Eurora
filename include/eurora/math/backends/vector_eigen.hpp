#pragma once

#include <Eigen/Dense>

#include "eurora/math/types.h"

namespace eurora::math {

template <typename T>
struct VectorEigenBackend {

    static Vector<T> Add(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());

        Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenResult(result.data(), result.size());
        Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenA(a.data(), a.size());
        Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenB(b.data(), b.size());

        eigenResult = eigenA + eigenB;

        return result;
    }

    static Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());

        Eigen::Map<Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenResult(result.data(), result.size());
        Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenA(a.data(), a.size());
        Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> eigenB(b.data(), b.size());

        eigenResult = eigenA - eigenB;

        return result;
    }
};

}  // namespace eurora::math
