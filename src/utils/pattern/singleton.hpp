#pragma once

#include "eurora/utils/export_macros.h"

namespace eurora::utils {

template <typename T>
class Singleton {
public:
    static T& Instance() noexcept {
        static T instance{Token{}};
        return instance;
    }

    Singleton(const Singleton&)            = delete;
    Singleton& operator=(const Singleton&) = delete;

protected:
    struct Token {};

    Singleton() {}
};

}  // namespace eurora::utils
