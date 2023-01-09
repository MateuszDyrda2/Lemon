/** @file hash_string.h
 * @brief File containing hash string implementation
 */
#pragma once

#include "assert.h"
#include "defines.h"
#include "lemon_types.h"

#include <functional>
#include <string>
#include <vector>

namespace lemon {
using hash_str = u32;
namespace detail {
static consteval hash_str hash_string_impl(const char* str, hash_str val) noexcept
{
    return (*str == 0) ? val : hash_string_impl(str + 1, ((val << 5) + val) + *str);
}
} // namespace detail

/** @brief Structure representation of a hashed string */
struct LEMON_API hashstr
{
    /** @brief Creates a basic hashstring */
    constexpr hashstr() = default;
    /** @brief Creates a hash string from a string
     * @param str string to be hashed
     */
    consteval hashstr(const char* str):
        value((*str == 0 ? 0 : detail::hash_string_impl(str, 5381U))), str(str) { }
    /** @brief Creates a hash string from a string and a hash
     * @param str string and already calculated hash
     */
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
    /** @brief Computes a hash at a runtime
     * @param str string to be hashed
     * @return calculated hashstr
     */
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
[[nodiscard]] static consteval hashstr operator""_hs(const char* str, size_t) noexcept
{
    return hashstr(str);
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
