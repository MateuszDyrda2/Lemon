#include <lemon/core/string_id.h>

namespace lemon {
#if defined(LEMON_DEBUG)
string_id::string_map* get_string_map()
{
    static string_id::string_map localMap;
    return &localMap;
}

string_id::string_map* string_id::map = get_string_map();
#endif

string_id::string_id(hash_t value, const std::string& name):
    id(value)
{
#if defined(LEMON_DEBUG)
    map->insert(std::make_pair(id, name));
#endif
}
const char* string_id::get_string() const
{
#if defined(LEMON_DEBUG)
    return map->at(id).c_str();
#else
    return "";
#endif // LEMON_DEBUG
}
const std::string& string_id::get_cpp_string() const
{
#if defined(LEMON_DEBUG)
    return map->at(id);
#else
    return "";
#endif // LEMON_DEBUG
}
string_id::operator bool() const noexcept
{
    return id != 0u;
}
} // namespace lemon
