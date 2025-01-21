#include <array>
#include <functional>
#include <memory>
#include <stdexcept>

#include "eurora/math/types.h"

namespace eurora::math {

template <typename typename FunctionType>
class BackendManager {
public:
    static BackendManager& Instance() {
        static BackendManager instance;
        return instance;
    }

    void Register(BackendType backendType, FunctionType func) { backends_[static_cast<std::size_t>(backendType)] = func; }

    FunctionType Get(BackendType backendType) const {
        auto index = static_cast<std::size_t>(backendType);
        if (index >= backends_.size() || !backends_[index]) {
            throw std::runtime_error("Backend not registered.");
        }
        return backends_[index];
    }

private:
    BackendManager()                                                                 = default;
    std::array<FunctionType, static_cast<std::size_t>(BackendType::Count)> backends_ = {};
};

}  // namespace eurora::math
