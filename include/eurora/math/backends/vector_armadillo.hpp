#pragma once

#include <armadillo>

#include "eurora/math/types.h"

namespace eurora::math {

template <typename T>
struct VectorArmadilloBackend {

    static Vector<T> Add(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());
        arma::Col<T> armaResult(result.data(), result.size(), false);

        arma::Col<T> armaA(const_cast<T*>(a.data()), a.size(), false);
        arma::Col<T> armaB(const_cast<T*>(b.data()), b.size(), false);

        armaResult = armaA + armaB;

        return result;
    }

    static Vector<T> Subtract(const Vector<T>& a, const Vector<T>& b) {
        Vector<T> result(1, a.size());
        arma::Col<T> armaResult(result.data(), result.size(), false);

        arma::Col<T> armaA(const_cast<T*>(a.data()), a.size(), false);
        arma::Col<T> armaB(const_cast<T*>(b.data()), b.size(), false);

        armaResult = armaA - armaB;

        return result;
    }
};

}  // namespace eurora::math
