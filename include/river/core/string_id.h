#pragma once

#include "basic_types.h"
#include "logger.h"

#include <string>
#include <unordered_map>

namespace river {
#if defined RIVER_DEBUG
#    define DEBUG_CONSTEXPR
#else
#    define DEBUG_CONSTEXPR constexpr
#endif
class string_id
{
  public:
    using hash_t     = u32;
    using string_map = std::unordered_map<hash_t, std::string>;

  public:
    string_id():
        id(0) { }
    DEBUG_CONSTEXPR explicit string_id(const char* _str);
    DEBUG_CONSTEXPR explicit string_id(const std::string& _str);
    string_id(const string_id&) = default;
    string_id& operator=(const string_id&) = default;
    constexpr string_id(hash_t value):
        id(value) { }

    const std::string& get_string() const;
    constexpr hash_t get_id() const { return id; }
    bool operator==(const string_id& other) const;

    static DEBUG_CONSTEXPR hash_t hash_str(const char* _str);

  private:
#if defined(RIVER_DEBUG)
    static string_map map;
#endif
  private:
    hash_t id;
};
} // namespace river
namespace std {
using namespace river;
template<>
struct hash<string_id>
{
    std::size_t operator()(const string_id& k) const noexcept
    {
        return k.get_id();
    }
};
} // namespace std
