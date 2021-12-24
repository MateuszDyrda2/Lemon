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
     DEBUG_CONSTEXPR explicit string_id(const char* _str);
    DEBUG_CONSTEXPR explicit string_id(const std::string& _str);
    const std::string& get_string() const;
    hash_t get_id() const { return id; }
    bool operator==(const string_id& other) const;

    static DEBUG_CONSTEXPR hash_t hash_str(const char* _str);

  private:
    static string_map& get_map();

  private:
    hash_t id;
};
} // namespace river
namespace std {
using namespace river;
template<>
struct hash<string_id>
{
    std::size_t operator()(const string_id& k) const
    {
        return k.get_id();
    }
};
} // namespace std
