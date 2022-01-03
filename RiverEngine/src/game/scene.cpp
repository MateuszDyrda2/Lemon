#include <river/game/scene.h>

#include <river/core/assert.h>
#include <river/game/basic_components.h>

#include <algorithm>

namespace river {
scene::scene(string_id name, ptr<rendering_context> context):
    object(name), registry(),
    scriptingSystem(create_owned<scripting_system>(this)),
    renderingSystem(create_owned<rendering_system>(this, context)),
    transformSystem(create_owned<transform_system>(this))
{
}
scene::~scene()
{
}
void scene::initialize()
{
}
void scene::begin()
{
}
void scene::update(float dt)
{
    scriptingSystem->update(registry, dt);
    transformSystem->update(registry);
    renderingSystem->render(registry);
}
entity scene::add_entity(string_id name)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, entity parent)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, parent.get_handle());
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const glm::vec3 position,
                         const glm::vec3 scale, f32 rotation)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, entt::null, entt::null);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const glm::vec3 position,
                         const glm::vec3 scale, f32 rotation, entity parent)
{
    auto ent   = registry.create();
    auto order = parent.get_component<transform>().order + 1;
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, parent.get_handle(),
                                order);
    return entity(&registry, ent);
}
entity scene::clone_entity(entity ent, string_id name)
{
    auto cloned_handle = registry.create();
    entity cloned(&registry, cloned_handle);
    for(auto&& curr : registry.storage())
        if(auto& storage = curr.second; storage.contains(ent.get_handle()))
            storage.emplace(cloned.get_handle(), storage.get(ent.get_handle()));
    registry.replace<tag>(cloned_handle, name);
    return cloned;
}
void scene::remove_entity(entity& ent)
{
    registry.destroy(ent.get_handle());
}
} // namespace river
