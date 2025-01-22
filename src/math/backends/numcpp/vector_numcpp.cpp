#include "vector_numcpp.h"

namespace eurora::math {

fvec VectorNumCppBackend::Add(const fvec& a, const fvec& b) { return a + b; }

fvec VectorNumCppBackend::Subtract(const fvec& a, const fvec& b) { return a - b; }

}  // namespace eurora::math
