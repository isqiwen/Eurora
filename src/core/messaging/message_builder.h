#pragma once

#include "Message.h"

namespace Core::Messaging {

class MessageBuilder {
public:
    template <typename... Args>
    static Message build(Args&&... args) {
        return Message(std::forward<Args>(args)...);
    }
};

}  // namespace Core::Messaging
