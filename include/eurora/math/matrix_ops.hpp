#pragma once

#include "eurora/math/backend_manager.hpp"
#include "eurora/math/call_function.hpp"
#include "eurora/math/types.h"

namespace eurora::math {

class MatMul {
public:
    virtual ~MatMul()                                         = default;
    virtual fmat Multiply(const fmat& A, const fmat& B) const = 0;
};

class VecAdd {
public:
    virtual ~VecAdd()                                    = default;
    virtual fvec Add(const fvec& A, const fvec& B) const = 0;
};

template <typename Backend>
class MatrixOps {
public:
    static fmat Multiply(const fmat& A, const fmat& B) { return Backend::Multiply(A, B); }

    static fmat Add(const fmat& A, const fmat& B) { return Backend::Add(A, B); }
};

inline fmat Multiply(const fmat& A, const fmat& B, BackendType backendType = BackendType::NumCpp) { return MatrixOps<>::Multiply(A, B); }

inline fvec Add(const fvec& A, const fvec& B, BackendType backendType = BackendType::NumCpp) { return CallFunction(&VecAdd::Add, backendType, A, B); }

}  // namespace eurora::math
