#include <lemon/game/object.h>

namespace lemon {
object::object(string_id id):
    id(id)
{ }
bool object::operator==(const object& other)
{
    return id == other.id;
}
} // namespace lemon
