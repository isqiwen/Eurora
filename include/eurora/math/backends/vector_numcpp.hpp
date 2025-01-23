#pragma once

#include "eurora/math/types.h"

namespace eurora::math {

template <typename T>
struct VectorNumCppBackend {

    static Vector<T> Add(const Vector<T>& a, const Vector<T>& b) { return a + b; }

    static Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) { return a - b; }
};

}  // namespace eurora::math
