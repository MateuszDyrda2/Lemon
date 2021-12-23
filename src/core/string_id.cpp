#include <river/core/string_id.h>

namespace river {
DEBUG_CONSTEXPR string_id::string_id(const char* _str):
    id(hash_str(_str))
{
#if defined(RIVER_DEBUG)
    auto& map = get_map();
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
DEBUG_CONSTEXPR string_id::string_id(const std::string& _str):
    string_id(_str.c_str())
{ }
static constexpr string_id::hash_t hash_str_impl(string_id::hash_t _hash, const char* _str)
{
    return (*_str == 0) ? _hash : hash_str_impl(((_hash << 5) + _hash) + *_str, _str + 1);
}
DEBUG_CONSTEXPR string_id::hash_t string_id::hash_str(const char* _str)
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
const std::string& string_id::get_string() const
{
#if defined(RIVER_DEBUG)
    return get_map().at(id);
#else
    return "";
#endif // RIVER_DEBUG
}
bool string_id::operator==(const string_id& other) const
{
    return id == other.id;
}
string_id::string_map& string_id::get_map()
{
    static string_map map;
    return map;
}
} // namespace river
