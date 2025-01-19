#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#define ADAPT_STRUCT(STRUCT, ...) \
    constexpr auto reflect_members() { return std::make_tuple(__VA_ARGS__); }

#define ACCESS_ELEMENT(ELEMENT) std::make_pair(std::string_view(#ELEMENT), [](auto& instance) -> auto& { return instance.ELEMENT; })
