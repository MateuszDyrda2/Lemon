#pragma once

#include <concepts>
#include <utility>

namespace lemon {
template<class Enum>
concept EnumT = std::is_enum_v<Enum>;

template<EnumT E>
constexpr inline std::underlying_type_t<E> enum_undelying(const E& val)
{
    return static_cast<typename std::underlying_type_t<E>>(val);
}
}
