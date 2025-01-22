#include "vector_armadillo.h"

#include <armadillo>

namespace eurora::math {

fvec VectorArmadilloBackend::Add(const fvec& a, const fvec& b) {
    fvec result(1, a.size());
    arma::fvec armaResult(result.data(), result.size(), false);

    arma::fvec armaA(const_cast<float*>(a.data()), a.size(), false);
    arma::fvec armaB(const_cast<float*>(b.data()), b.size(), false);

    armaResult = armaA + armaB;

    return result;
}

fvec VectorArmadilloBackend::Subtract(const fvec& a, const fvec& b) {
    fvec result(1, a.size());
    arma::fvec armaResult(result.data(), result.size(), false);

    arma::fvec armaA(const_cast<float*>(a.data()), a.size(), false);
    arma::fvec armaB(const_cast<float*>(b.data()), b.size(), false);

    armaResult = armaA - armaB;

    return result;
}

}  // namespace eurora::math
