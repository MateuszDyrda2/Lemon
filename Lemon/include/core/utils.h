/** @file utils.h
 * @brief File with utility functions
 */
#pragma once

#include <concepts>
#include <utility>

namespace lemon {
template<class Enum>
concept EnumT = std::is_enum_v<Enum>;

/** @brief Get enum value in its underlying type
 * @param val value to be cast
 * @return converted value
 * @tparam E enum type
 */
template<EnumT E>
constexpr inline std::underlying_type_t<E> enum_undelying(const E& val)
{
    return static_cast<typename std::underlying_type_t<E>>(val);
}
/** @brief Check if a value is in range
 * @param val value to check
 * @param low lower bound
 * @param high higher bound
 * @return true if its in range
 */
template<class T>
constexpr inline bool in_range(const T& val, const T& low, const T& high)
{
    return !(val < low) && !(high < val);
}
/** @brief Run callable for each of the arguments
 * @param callable function object to be called on each of the arguments
 * @param args objects the function should be called on
 * @tparam F callable type should be able to be called on each of the args
 * @tparam Args object types the function can take
 */
template<class F, class... Args>
inline void run_for_each(F callable, Args&&... args)
{
    (callable(args), ...);
}
}
