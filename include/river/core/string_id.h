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
    DEBUG_CONSTEXPR string_id(char const* _str);
    DEBUG_CONSTEXPR string_id(std::string const& _str);
    std::string const& get_string() const;
    hash_t get_id() const { return id; }
    bool operator==(string_id const& other) const;

    static DEBUG_CONSTEXPR hash_t hash_str(char const* _str);

  private:
    static string_map& get_map();

  private:
    hash_t id;
};
} // namespace river