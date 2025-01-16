#pragma once

#include <memory>
#include <vector>
#include "MessageChunk.h"

namespace Core::Messaging {

class Message {
public:
    template <typename... Args>
    explicit Message(Args&&... args) {
        (add(std::forward<Args>(args)), ...);
    }

    template <typename T>
    void add(T&& data) {
        chunks_.emplace_back(std::make_unique<TypedMessageChunk<std::decay_t<T>>>(std::forward<T>(data)));
    }

    const std::vector<std::unique_ptr<MessageChunk>>& chunks() const { return chunks_; }

    std::vector<std::unique_ptr<MessageChunk>> take_chunks() { return std::move(chunks_); }

private:
    std::vector<std::unique_ptr<MessageChunk>> chunks_;
};

}  // namespace Core::Messaging
