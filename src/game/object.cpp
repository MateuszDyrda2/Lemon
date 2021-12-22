#include <river/game/object.h>

namespace river {
object::object(string_id id):
    id(id)
{ }
object::object(std::string const& name):
    id(name)
{ }
bool object::operator==(object const& other)
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
