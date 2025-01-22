#pragma once

#include "eurora/math/types.h"

namespace eurora::math {

struct VectorEigenBackend {

    static fvec Add(const fvec& a, const fvec& b);

    static fvec Subtract(const fvec& a, const fvec& b);
};

}  // namespace eurora::math
