#include <algorithm>
#include <river/game/scene.h>

namespace river {
scene::scene(std::string const& name):
    object(name)
{
}
scene::~scene()
{
}
void scene::update(float deltaTime)
{
    for(auto& e : entities)
    {
        e->update(deltaTime);
    }
    for(auto& e : toDestroy)
    {
        e->on_disable();
        std::erase(entities, e);
    }
    toDestroy.clear();
}
void scene::fixed_update(float fixedDelta)
{
    for(auto& e : entities)
    {
        e->fixed_update(fixedDelta);
    }
}
void scene::late_update(float deltaTime)
{
    for(auto& e : entities)
    {
        e->late_update(deltaTime);
    }
}
std::shared_ptr<entity> const& scene::add_entity(std::shared_ptr<entity> const& ent)
{
    entities.push_back(ent);
    return ent;
}
std::shared_ptr<entity> const& scene::add_entity(std::string const& name)
{
    return entities.emplace_back(name);
}
} // namespace river
