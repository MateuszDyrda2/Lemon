/** @file hash_string.h
 * @brief File containing hash string implementation
 */
#pragma once

#include "assert.h"
#include "lemon_types.h"

namespace lemon {
using hash_str = u32;
namespace detail {
static consteval hash_str hash_string_impl(const char* str, hash_str val) noexcept
{
    return (*str == 0) ? val : hash_string_impl(str + 1, ((val << 5) + val) + *str);
}
} // namespace detail

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
[[nodiscard]] static consteval hash_str operator""_hs(const char* str, size_t) noexcept
{
    return (*str == 0) ? 0 : detail::hash_string_impl(str, 5381U);
}
/** @brief Creates a string hash at runtime for id comparison
 * @param str null terminated string
 * @return hashed id
 */
[[nodiscard]] static hash_str hash_string_d(const char* str) noexcept
{
    if(*str == 0) return 0;
    hash_str hash = 5381;
    char c;
    while((c = *str++) != 0) hash = ((hash << 5) + hash) + c;
    return hash;
}
} // namespace lemon
