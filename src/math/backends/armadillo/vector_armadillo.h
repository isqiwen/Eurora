#pragma once

#include "eurora/utils/export_macros.h"

#include "eurora/math/types.h"

namespace eurora::math {

struct EURORA_API VectorArmadilloBackend {

    static fvec Add(const fvec& a, const fvec& b);

    static fvec Subtract(const fvec& a, const fvec& b);
};

}  // namespace eurora::math
