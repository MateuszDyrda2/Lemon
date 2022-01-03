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
    DEBUG_CONSTEXPR explicit string_id(const char* _str):
        id(hash_str(_str))
    {
#if defined(RIVER_DEBUG)
        if(map.count(id) == 1)
        {
            if(map.at(id) != _str)
                LOG_FATAL("String hash conflict! %s and %s have the same hash!", map.at(id), _str);
        }
        else
        {
            map.insert({ id, _str });
        }
#endif // RIVER_DEBUG
    }
    string_id(const string_id&) = default;
    string_id& operator=(const string_id&) = default;
    constexpr string_id(hash_t value):
        id(value) { }

    const char* get_string() const;
    constexpr hash_t get_id() const { return id; }
    bool operator==(const string_id& other) const;

    static DEBUG_CONSTEXPR hash_t hash_str(const char* _str)
    {
#if defined(RIVER_DEBUG)
        if(*_str == 0) return 0;

        hash_t hash = 5381;
        char c;
        while((c = *_str++) != 0) hash = ((hash << 5) + hash) + c;
        return hash;
#else
        return (*_str == 0) ? 0 : hash_str_impl(5381, _str);

#endif // RIVER_DEBUG
    }

  private:
    static constexpr hash_t hash_str_impl(string_id::hash_t _hash, const char* _str)
    {
        return (*_str == 0) ? _hash : hash_str_impl(((_hash << 5) + _hash) + *_str, _str + 1);
    }
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
