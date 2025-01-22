#pragma once

namespace eurora::math {

template <typename Operations, typename Operations::FunctionTag Tag>
struct FunctionTagDefinition {
    using Type = typename Operations::template FunctionTagDefinition<Tag>::Type;
};

template <typename Operations, typename Backend>
struct FunctionTableGenerator {
public:
    template <typename Operations::FunctionTag Tag>
    static constexpr auto Generate() {
        using FunctionPtr = typename FunctionTagDefinition<Operations, Tag>::Type;

        std::array<FunctionPtr, static_cast<size_t>(BackendType::Count)> table = {};

        for (size_t i = 0; i < static_cast<size_t>(BackendType::Count); ++i) {
            BackendType backendType = static_cast<BackendType>(i);

            if constexpr (Operations::template IsFunctionImplemented<Backend, Tag>::value) {
                if (backendType == BackendType::Eigen) {
                    table[i] = &Backend::template GetFunction<Tag>;
                }
                // Add other backends as needed
            }
        }

        return table;
    }
};

}  // namespace eurora::math
