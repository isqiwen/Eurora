#include "vector_eigen.h"

#include <Eigen/Dense>

namespace eurora::math {

fvec VectorEigenBackend::Add(const fvec& a, const fvec& b) {
    fvec result(1, a.size());
    Eigen::Map<Eigen::VectorXf> eigenResult(result.data(), result.size());

    Eigen::Map<const Eigen::VectorXf> eigenA(a.data(), a.size());
    Eigen::Map<const Eigen::VectorXf> eigenB(b.data(), b.size());
    eigenResult = eigenA + eigenB;

    return result;
}

fvec VectorEigenBackend::Subtract(const fvec& a, const fvec& b) {
    fvec result(1, a.size());
    Eigen::Map<Eigen::VectorXf> eigenResult(result.data(), result.size());

    Eigen::Map<const Eigen::VectorXf> eigenA(a.data(), a.size());
    Eigen::Map<const Eigen::VectorXf> eigenB(b.data(), b.size());
    eigenResult = eigenA - eigenB;

    return result;
}

}  // namespace eurora::math
