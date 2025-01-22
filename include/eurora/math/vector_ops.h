#pragma once

#include "eurora/math/types.h"

namespace eurora::math {

struct VectorOperations {
    enum class FunctionTag { Add, Subtract };

    template <typename VectorBackend, FunctionTag Tag, typename = void>
    struct IsFunctionImplemented : std::false_type {};

    template <typename VectorBackend>
    struct IsFunctionImplemented<VectorBackend, FunctionTag::Add, std::void_t<decltype(&VectorBackend::Add)>> : std::true_type {};

    template <typename VectorBackend>
    struct IsFunctionImplemented<VectorBackend, FunctionTag::Subtract, std::void_t<decltype(&VectorBackend::Subtract)>> : std::true_type {};

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

    template <typename Backend>
    static auto Add(const fvec& a, const fvec& b) {
        using VectorBackend = VectorBackendMapping<Backend>::Type;
        static_assert(IsFunctionImplemented<VectorBackend, FunctionTag::Add>::value, "Backend does not implement Add.");
        return VectorBackend::Add(a, b);
    }

    template <typename Backend>
    static auto Subtract(const fvec& a, const fvec& b) {
        using VectorBackend = VectorBackendMapping<Backend>::Type;
        static_assert(IsFunctionImplemented<VectorBackend, FunctionTag::Subtract>::value, "Backend does not implement Subtract.");
        return VectorBackend::Subtract(a, b);
    }
};

}  // namespace eurora::math
