#include <river/game/object.h>

namespace river {
object::object(string_id id):
    id(id)
{ }
object::object(const std::string& name):
    id(name)
{ }
bool object::operator==(const object& other)
{
    return id == other.id;
}
void object::destroy(object* obj)
{
    obj->destroy_this();
}
object* object::clone(object* obj)
{
    return obj->clone_this();
}
} // namespace river
