#include <lemon/core/string_id.h>

namespace lemon {
#if defined(LEMON_DEBUG)
string_id::string_map string_id::map;
#endif

const char* string_id::get_string() const
{
#if defined(LEMON_DEBUG)
    return map.at(id).c_str();
#else
    return "";
#endif // LEMON_DEBUG
}
bool string_id::operator==(const string_id& other) const
{
    return id == other.id;
}
} // namespace lemon
