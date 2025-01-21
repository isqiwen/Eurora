#include "eurora/math/backend_manager.hpp"
#include "eurora/math/matrix_ops.hpp"

namespace eurora::math {

class MatrixOpsEigen {
public:
    fmat Multiply(const fmat& A, const fmat& B) const override { return fmat(); }
};

namespace {
const bool reg_mat_mul_backends = [] {
    auto& manager = BackendManager<MatMul>::Instance();
    manager.Register(BackendType::MKL, std::make_unique<MatMulMKL>());
    manager.Register(BackendType::Eigen, std::make_unique<MatMulEigen>());
    return true;
}();
}  // namespace

}  // namespace eurora::math
