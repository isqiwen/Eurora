#pragma once

#include <stdexcept>
#include <tuple>
#include "Message.h"

namespace Core::Messaging {

class MessageUnpacker {
public:
    template <typename... Types>
    static std::tuple<Types...> unpack(const Message& message) {
        const auto& chunks = message.chunks();
        if (chunks.size() != sizeof...(Types)) {
            throw std::runtime_error("Message size does not match expected types");
        }

        size_t index = 0;
        return unpack_impl<Types...>(chunks, index);
    }

private:
    template <typename T, typename... Rest>
    static std::tuple<T, Rest...> unpack_impl(const std::vector<std::unique_ptr<MessageChunk>>& chunks, size_t& index) {
        const auto& chunk = chunks[index++];
        if (chunk->type() != typeid(T)) {
            throw std::runtime_error("Type mismatch during unpacking");
        }

        auto& typed_chunk = static_cast<const TypedMessageChunk<T>&>(*chunk);
        return std::tuple_cat(std::make_tuple(typed_chunk.data()), unpack_impl<Rest...>(chunks, index));
    }

    template <typename T>
    static std::tuple<T> unpack_impl(const std::vector<std::unique_ptr<MessageChunk>>& chunks, size_t& index) {
        const auto& chunk = chunks[index++];
        if (chunk->type() != typeid(T)) {
            throw std::runtime_error("Type mismatch during unpacking");
        }

        auto& typed_chunk = static_cast<const TypedMessageChunk<T>&>(*chunk);
        return std::make_tuple(typed_chunk.data());
    }
};

}  // namespace Core::Messaging
