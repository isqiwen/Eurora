#pragma once

#include "eurora/math/backend_type.h"
#include "eurora/math/types.h"

#include "math/backends/armadillo/vector_armadillo.h"
#include "math/backends/eigen/vector_eigen.h"
#include "math/backends/numcpp/vector_numcpp.h"

namespace eurora::math {

template <BackendType backendType>
struct VectorBackendMapping;

template <>
struct VectorBackendMapping<BackendType::Eigen> {
    using Type = VectorEigenBackend;
};

template <>
struct VectorBackendMapping<BackendType::Armadillo> {
    using Type = VectorArmadilloBackend;
};

template <>
struct VectorBackendMapping<BackendType::NumCpp> {
    using Type = VectorNumCppBackend;
};

template <typename Backend>
concept AddValidBackend = requires(const fvec& a, const fvec& b) {
    { Backend::Add(a, b) } -> std::same_as<fvec>;
};

template <typename Backend>
concept SubtractValidBackend = requires(const fvec& a, const fvec& b) {
    { Backend::Subtract(a, b) } -> std::same_as<fvec>;
};

struct VectorOperations {

    template <BackendType backendType>
    static fvec Add(const fvec& a, const fvec& b) requires AddValidBackend<typename VectorBackendMapping<backendType>::Type> {
        using VectorBackend = typename VectorBackendMapping<backendType>::Type;
        return VectorBackend::Add(a, b);
    }

    template <BackendType backendType>
    static fvec Subtract(const fvec& a, const fvec& b) requires SubtractValidBackend<typename VectorBackendMapping<backendType>::Type> {
        using VectorBackend = typename VectorBackendMapping<backendType>::Type;
        return VectorBackend::Subtract(a, b);
    }
};

template <BackendType Blackend = BackendType::NumCpp>
fvec Add(const fvec& a, const fvec& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have the same size for add.");
    }

    return VectorOperations::Add<Blackend>(a, b);
}

template <BackendType Blackend = BackendType::NumCpp>
fvec Subtract(const fvec& a, const fvec& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have the same size for subtraction.");
    }

    return VectorOperations::Subtract<Blackend>(a, b);
}

}  // namespace eurora::math
