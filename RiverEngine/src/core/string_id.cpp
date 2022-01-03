#include <river/core/string_id.h>

namespace river {
#if defined(RIVER_DEBUG)
string_id::string_map string_id::map;
#endif

const char* string_id::get_string() const
{
#if defined(RIVER_DEBUG)
    return map.at(id).c_str();
#else
    return "";
#endif // RIVER_DEBUG
}
bool string_id::operator==(const string_id& other) const
{
    return id == other.id;
}
} // namespace river
