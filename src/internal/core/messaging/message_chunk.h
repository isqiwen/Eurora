#pragma once

#include <memory>
#include <typeindex>
#include <typeinfo>

namespace Core::Messaging {

class MessageChunk {
public:
    virtual ~MessageChunk() = default;

    // Clone the chunk
    virtual std::unique_ptr<MessageChunk> clone() const = 0;

    // Return the type information
    virtual const std::type_info& type() const = 0;
};

template <typename T>
class TypedMessageChunk : public MessageChunk {
public:
    explicit TypedMessageChunk(T data) : data_(std::move(data)) {}

    const T& data() const { return data_; }

    T& data() { return data_; }

    std::unique_ptr<MessageChunk> clone() const override { return std::make_unique<TypedMessageChunk<T>>(data_); }

    const std::type_info& type() const override { return typeid(T); }

private:
    T data_;
};

}  // namespace Core::Messaging
