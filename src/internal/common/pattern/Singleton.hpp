#ifndef EURORA_COMMON_PATTERN_SINGLETON_
#define EURORA_COMMON_PATTERN_SINGLETON_

#pragma once

namespace eurora::common {

template<typename T>
class Singleton
{
  public:
    static T &instance()
    {
        static T instance{ token{} };
        return instance;
    }

    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton) = delete;

  protected:
    struct token {
    };
    Singleton()
    {}
};

}  // namespace eurora::common

#endif  // EURORA_COMMON_PATTERN_SINGLETON_
