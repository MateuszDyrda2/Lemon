/** @file hash_string.h
 * @brief File containing hash string implementation
 */
#pragma once

#include "assert.h"
#include "defines.h"
#include "lemon_types.h"
#include <functional>
#include <vector>

namespace lemon {
using hash_str = u32;
namespace detail {
static consteval hash_str hash_string_impl(const char* str, hash_str val) noexcept
{
    return (*str == 0) ? val : hash_string_impl(str + 1, ((val << 5) + val) + *str);
}
} // namespace detail

struct LEMON_API hashstr
{
    constexpr hashstr() = default;
    consteval hashstr(const char* str):
        value((*str == 0 ? 0 : detail::hash_string_impl(str, 5381U))), str(str) { }

    constexpr hashstr(const char* str, u32 value):
        value(value), str(str) { }

    constexpr bool operator==(const hashstr& other) const
    {
        return value == other.value;
    }

    constexpr bool operator!=(const hashstr& other) const
    {
        return !(*this == other);
    }

    static hashstr runtime_hash(const char* str)
    {
        if (*str == 0) return (hashstr(str, 0));
        u32 hash = 5381;
        runtimeStrings.push_back(str);
        char c;
        while ((c = *str++) != 0) hash = ((hash << 5) + hash) + c;
        const char* cpy = runtimeStrings.back().c_str();
        return hashstr(cpy, hash);
    }

    explicit constexpr operator bool() const
    {
        return str != 0;
    }

    u32 value{};
    const char* str;

    inline static std::vector<std::string> runtimeStrings;
};

/** @brief Creates a string hash for id comparison
 * @param str null terminated string
 * @return hashed id
 */
[[nodiscard]] static consteval hash_str hash_string(const char* str) noexcept
{
    return (*str == 0) ? 0 : detail::hash_string_impl(str, 5381U);
}
/** @brief Creates a string hash for id comparison
 * @param str null terminated string
 * @return hashed id
 */
[[nodiscard]] static consteval hashstr operator""_hs(const char* str, size_t) noexcept
{
    return hashstr(str);
}
/** @brief Creates a string hash at runtime for id comparison
 * @param str null terminated string
 * @return hashed id
 */
[[nodiscard]] static __attribute__((unused)) hash_str hash_string_d(const char* str) noexcept
{
    if (*str == 0) return 0;
    hash_str hash = 5381;
    char c;
    while ((c = *str++) != 0) hash = ((hash << 5) + hash) + c;
    return hash;
}
} // namespace lemon

namespace std {
template<>
struct hash<lemon::hashstr>
{
    std::size_t operator()(const lemon::hashstr& k) const
    {
        using std::hash;
        using std::size_t;
        using std::string;

        return hash<lemon::u32>()(k.value);
    }
};
}
