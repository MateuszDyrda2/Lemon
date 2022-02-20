#include <lemon/core/string_id.h>

namespace lemon {
#if defined(LEMON_DEBUG)
string_id::string_map string_id::map;
#endif

string_id::string_id(std::nullptr_t):
    id(0u)
{
}
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
string_id::operator bool() const noexcept
{
    return id != 0u;
}
} // namespace lemon
