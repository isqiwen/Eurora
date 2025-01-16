#pragma once

#include <functional>
#include <typeindex>
#include <unordered_map>
#include "Message.h"

namespace Core::Messaging {

class MessageDispatcher {
public:
    template <typename T>
    void subscribe(std::function<void(const T&)> handler) {
        handlers_[typeid(T)] = [handler = std::move(handler)](const MessageChunk& chunk) { handler(static_cast<const TypedMessageChunk<T>&>(chunk).data()); };
    }

    void dispatch(const Message& message) const {
        for (const auto& chunk : message.chunks()) {
            auto it = handlers_.find(chunk->type());
            if (it != handlers_.end()) {
                it->second(*chunk);
            }
        }
    }

private:
    std::unordered_map<std::type_index, std::function<void(const MessageChunk&)>> handlers_;
};

}  // namespace Core::Messaging
