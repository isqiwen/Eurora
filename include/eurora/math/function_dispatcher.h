#pragma once

namespace ms {

enum class BackendType : size_t { MKL = 0, Eigen, Armadillo, NumCpp, Count };

struct VectorEigenBackend {

    static int Add(int a, int b) { return a + b; }

    static int Subtract(int a, int b) { return a - b; }
};

struct VectorMKLBackend {

    static int Add(int a, int b) { return a + b; }

    static int Subtract(int a, int b) { return a - b; }
};

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
concept ValidBackend = requires(int a, int b) {
    { Backend::Add(a, b) } -> std::same_as<int>;
};

struct VectorOperations {
    enum class FunctionTag { Add, Subtract };

    template <typename VectorBackend, FunctionTag Tag, typename = void>
    struct IsFunctionImplemented : std::false_type {};

    template <typename VectorBackend>
    struct IsFunctionImplemented<VectorBackend, FunctionTag::Add, std::void_t<decltype(&VectorBackend::Add)>> : std::true_type {};

    template <typename VectorBackend>
    struct IsFunctionImplemented<VectorBackend, FunctionTag::Subtract, std::void_t<decltype(&VectorBackend::Subtract)>> : std::true_type {};

    // template <BackendType backendType>
    // static auto Add(int a, int b) {
    //     using VectorBackend = VectorBackendMapping<backendType>::Type;
    //     static_assert(IsFunctionImplemented<VectorBackend, FunctionTag::Add>::value, "Backend does not implement Add.");
    //     return VectorBackend::Add(a, b);
    // }

    template <BackendType backendType>
    requires ValidBackend<typename VectorBackendMapping<backendType>::Type> static auto Add(int a, int b) {
        using VectorBackend = VectorBackendMapping<backendType>::Type;
        return VectorBackend::Add(a, b);
    }

    template <BackendType backendType>
    static auto Subtract(int a, int b) {
        using VectorBackend = VectorBackendMapping<backendType>::Type;
        static_assert(IsFunctionImplemented<VectorBackend, FunctionTag::Subtract>::value, "Backend does not implement Subtract.");
        return VectorBackend::Subtract(a, b);
    }
};

}  // namespace ms
