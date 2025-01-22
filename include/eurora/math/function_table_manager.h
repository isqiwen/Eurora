#pragma once

#include "eruora/math/vector_ops.h"
#include "eurora/math/backend/vector_eigen_backend.h"

namespace eurora::math {



struct FunctionTableManager {
    static constexpr auto AddTable      = GenerateAddFunctionTable<AddFunctionEigenBackend>();
    static constexpr auto MultiplyTable = GenerateMultiplyFunctionTable<MultiplyFunctionEigenBackend>();

    static auto GetAddFunction(BackendType backend) { return AddTable[static_cast<size_t>(backend)]; }

    static auto GetMultiplyFunction(BackendType backend) { return MultiplyTable[static_cast<size_t>(backend)]; }
};

}  // namespace eurora::math
