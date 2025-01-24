#pragma once

#include "eurora/math/backend_type.h"
#include "eurora/math/types.h"

#include "eurora/math/backends/vector_armadillo.hpp"
#define _MKL_LAPACK_H_
#include "mkl.h"
#include "eurora/math/backends/vector_eigen.hpp"
#include "eurora/math/backends/vector_numcpp.hpp"
#include "eurora/math/backends/vector_mkl.hpp"

namespace eurora::math {

template <BackendType backendType>
struct VectorBackendMapping;

template <>
struct VectorBackendMapping<BackendType::MKL> {
    template <typename T>
    using Type = VectorMKLBackend<T>;
};

template <>
struct VectorBackendMapping<BackendType::Eigen> {
    template <typename T>
    using Type = VectorEigenBackend<T>;
};

template <>
struct VectorBackendMapping<BackendType::Armadillo> {
    template <typename T>
    using Type = VectorArmadilloBackend<T>;
};

template <>
struct VectorBackendMapping<BackendType::NumCpp> {
    template <typename T>
    using Type = VectorNumCppBackend<T>;
};

template <typename T, BackendType backendType>
concept AddValidBackend = requires(const Vector<T>& a, const Vector<T>& b) {
    { VectorBackendMapping<backendType>::template Type<T>::Add(a, b) } -> std::same_as<Vector<T>>;
};

template <typename T, BackendType backendType>
concept SubtractValidBackend = requires(const Vector<T>& a, const Vector<T>& b) {
    { VectorBackendMapping<backendType>::template Type<T>::Subtract(a, b) } -> std::same_as<Vector<T>>;
};

struct VectorOperations {
    template <typename T, BackendType backendType>
    static Vector<T> Add(const Vector<T>& a, const Vector<T>& b) requires AddValidBackend<T, backendType> {
        using Backend = typename VectorBackendMapping<backendType>::template Type<T>;
        return Backend::Add(a, b);
    }

    template <typename T, BackendType backendType>
    static Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) requires SubtractValidBackend<T, backendType> {
        using Backend = typename VectorBackendMapping<backendType>::template Type<T>;
        return Backend::Subtract(a, b);
    }
};

template <typename T, BackendType backendType = BackendType::NumCpp>
Vector<T> Add(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have the same size for add.");
    }

    return VectorOperations::Add<T, backendType>(a, b);
}

template <typename T, BackendType backendType = BackendType::NumCpp>
Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have the same size for subtraction.");
    }

    return VectorOperations::Subtract<T, backendType>(a, b);
}

}  // namespace eurora::math
