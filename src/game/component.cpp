#include <river/game/component.h>

#include <river/game/entity.h>

namespace river {
component::component(entity* _ent, size_type idx, bool enabled):
    object(_ent->get_id()), _entity(_ent), _entityIndex(idx), enabled(enabled)
{ }
bool component::is_enabled() const
{
    return !!(enabled);
}
bool component::is_active_and_enabled() const
{
    return !!(enabled & active);
}
void component::destroy_this()
{
    _entity->destroy_component(_entityIndex);
}
object* component::clone_this()
{
    return new component(*this);
}
void component::enable()
{
    if(!enabled)
    {
        enabled = 1;
        on_enable();
    }
}
void component::disable()
{
    if(enabled)
    {
        enabled = 0;
        on_disable();
    }
}
} // namespace river