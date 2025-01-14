#ifndef EURORA_COMMON_PATTERN_SINGLETON_H_
#define EURORA_COMMON_PATTERN_SINGLETON_H_

#pragma once

namespace eurora::common {

template <typename T>
class Singleton {
public:
  static T& Instance() {
    static T instance{Token{}};
    return instance;
  }

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton) = delete;

protected:
  struct Token {};

  Singleton() {}
};

}  // namespace eurora::common

#endif  // EURORA_COMMON_PATTERN_SINGLETON_H_
