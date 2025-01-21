#pragma once

#include <NumCpp.hpp>
#include <complex>

namespace eurora::math {

template <typename T>
using Vector = nc::NdArray<T>;

template <typename T>
using Matrix = nc::NdArray<T>;

template <typename T>
using Cube = nc::NdArray<T>;

template <typename T>
using Tensor = nc::NdArray<T>;

using fvec      = Vector<float>;
using dvec      = Vector<double>;
using int32vec  = Vector<int32_t>;
using uint32vec = Vector<uint32_t>;
using cx_fvec   = Vector<std::complex<float>>;
using cx_dvec   = Vector<std::complex<double>>;

using fmat      = Matrix<float>;
using dmat      = Matrix<double>;
using int32mat  = Matrix<int32_t>;
using uint32mat = Matrix<uint32_t>;
using uint16mat = Matrix<uint16_t>;
using int16mat  = Matrix<int16_t>;
using cx_fmat   = Matrix<std::complex<float>>;
using cx_dmat   = Matrix<std::complex<double>>;

using fcube      = Cube<float>;
using dcube      = Cube<double>;
using int32cube  = Cube<int32_t>;
using uint32cube = Cube<uint32_t>;
using cx_fcube   = Cube<std::complex<float>>;
using cx_dcube   = Cube<std::complex<double>>;

using fTensor4D    = Tensor<float>;
using dTensor4D    = Tensor<double>;
using cx_fTensor4D = Tensor<std::complex<float>>;
using cx_dTensor4D = Tensor<std::complex<double>>;

enum class BackendType : size_t { MKL = 0, Eigen, Armadillo, NumCpp, Count };

}  // namespace eurora::math
