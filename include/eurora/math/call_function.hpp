#pragma once

template <typename ReturnType, typename... Args>
inline ReturnType CallFunction(ReturnType (*function)(Args...), BackendType backendType, Args&&... args) {
    auto backend = BackendManager<ReturnType (*)(Args...)>::Instance().Get(backendType);
    return backend(std::forward<Args>(args)...);
}
