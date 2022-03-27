#include <lemon/scene/scene.h>

#include <lemon/scene/components/audio_components.h>
#include <lemon/scene/components/rendering_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
scene::scene(string_id name, entity_registry&& registry):
    name(name), registry(std::move(registry))
{
    auto view  = this->registry.view<camera>();
    mainCamera = entity(&this->registry, view.front());
    mainCamera.add_component<audio_listener>(); // TODO: Remove this
}
scene::scene(string_id name):
    name(name), registry(), mainCamera(&registry, registry.create())
{
    mainCamera.add_component<tag>(string_id("MainCamera"));
    mainCamera.add_component<transform>();
    mainCamera.add_component<dirty>();
    mainCamera.add_component<camera>();
    mainCamera.add_component<audio_listener>();
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
void scene::update()
{
    for(auto& s : systems)
        s->update(registry);
}
entity scene::add_entity(string_id name)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent);
    registry.emplace<dirty>(ent);
    registry.emplace<enabled>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, entity parent)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, parent.get_handle());
    registry.emplace<dirty>(ent);
    registry.emplace<enabled>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const vec3& position,
                         const vec3& scale, f32 rotation)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, entt::null, entt::null);
    registry.emplace<dirty>(ent);
    registry.emplace<enabled>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const vec3& position,
                         const vec3& scale, f32 rotation, entity parent)
{
    auto ent   = registry.create();
    auto order = parent.get_component<transform>().order + 1;
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, parent.get_handle(),
                                order);
    registry.emplace<dirty>(ent);
    registry.emplace<enabled>(ent);
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
    registry.emplace_or_replace<dirty>(cloned_handle);
    return cloned;
}
void scene::remove_entity(entity& ent)
{
    registry.destroy(ent.get_handle());
}
} // namespace lemon
