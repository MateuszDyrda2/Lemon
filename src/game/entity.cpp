#include <river/game/entity.h>

namespace river {
entity::entity(std::string const& name):
    object(name)
{
}
entity::entity(string_id id):
    object(id)
{
}
entity::~entity()
{
}
void entity::on_enable()
{
    for(auto& c : components)
    {
        c->on_enable();
    }
}
void entity::update(float deltaTime)
{
    for(auto& c : components)
    {
        if(c->is_enabled()) c->update(deltaTime);
    }
    for(auto& c : toDestroy)
    {
        c->on_disable();
        c->on_destroy();
        std::erase(components, c);
    }
    toDestroy.clear();
}
void entity::fixed_update(float fixedDelta)
{
    for(auto& c : components)
    {
        if(c->is_enabled()) c->fixed_update(fixedDelta);
    }
}
void entity::late_update(float deltaTime)
{
    for(auto& c : components)
    {
        if(c->is_enabled()) c->late_update(deltaTime);
    }
}
void entity::on_disable()
{
    for(auto& c : components)
    {
        if(c->is_enabled()) c->on_disable();
    }
}
void entity::set_active(bool value)
{
    isActive = value;
}
void entity::destroy_this()
{
}
object* entity::clone_this()
{
}
} // namespace river
