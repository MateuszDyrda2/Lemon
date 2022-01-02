#include <river/game/object.h>

namespace river {
object::object(string_id id):
    id(id)
{ }
bool object::operator==(const object& other)
{
    return id == other.id;
}
} // namespace river
