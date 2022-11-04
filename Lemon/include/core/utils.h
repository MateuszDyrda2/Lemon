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

template<class T>
constexpr inline bool in_range(const T& val, const T& low, const T& high)
{
    return !(val < low) && !(high < val);
}

template<class F, class... Args>
inline void run_for_each(F callable, Args&&... args)
{
    (callable(args), ...);
}
}
