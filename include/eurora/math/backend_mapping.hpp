#pragma once

#include "backend_type.h"
#include "eurora/math/backend/vector_eigen_backend.h"

template <BackendType backendType>
struct VectorBackendMapping;

template <>
struct VectorBackendMapping<BackendType::Eigen> {
    using Type = VectorEigenBackend;
};

template <>
struct VectorBackendMapping<BackendType::MKL> {
    using Type = VectorMKLBackend;
};
