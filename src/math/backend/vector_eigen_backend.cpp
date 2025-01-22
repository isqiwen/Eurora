#pragma once

#include "eurora/math/backend/vector_eigen_backend.h"

namespace eurora::math {

fvec VectorEigenBackend::Add(const fvec& a, const fvec& b) {
    fvec result(a.data.size());
    for (size_t i = 0; i < a.data.size(); ++i) {
        result[i] = a[i] + b[i];
    }
    return result;
}

fvec VectorEigenBackend::Subtract(const fvec& a, const fvec& b) {
    fvec result(a.data.size());
    for (size_t i = 0; i < a.data.size(); ++i) {
        result[i] = a[i] - b[i];
    }
    return result;
}
};

}  // namespace eurora::math
