#include <lemon/scene/scene.h>

#include <lemon/core/instrumentor.h>
#include <lemon/scene/components/entity_components.h>
#include <lemon/scene/components/transform_components.h>

namespace lemon {
using namespace std;
scene::scene(string_id name, service_registry& globalRegistry, entity_registry&& registry):
    name(name), registry(std::move(registry)), globalRegistry(globalRegistry),
    clk(globalRegistry.get_service<game_clock>())
{
}
scene::scene(string_id name, service_registry& globalRegistry):
    name(name), registry(), globalRegistry(globalRegistry),
    clk(globalRegistry.get_service<game_clock>())
{
    /*
    mainCamera.add_component<tag>(string_id("MainCamera"));
    mainCamera.add_component<transform>();
    mainCamera.add_component<dirty>();
    mainCamera.add_component<camera>();
    mainCamera.add_component<audio_listener>();
    */
}
scene::~scene()
{
}
void scene::scene_load()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& s : systems)
    {
        s->on_scene_load(registry);
    }
}
void scene::frame_begin()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& [_, s] : onFrameBeginMap)
    {
        s->on_update(registry);
    }
}
void scene::physics_update()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& [_, s] : onPhysicsMap)
    {
        s->on_update(registry);
    }
}
void scene::update()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& [_, s] : onUpdateMap)
    {
        s->on_update(registry);
    }
}
void scene::frame_end()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& [_, s] : onFrameEndMap)
    {
        s->on_update(registry);
    }
}
void scene::scene_unload()
{
    LEMON_PROFILE_FUNCTION();
    for(auto& s : systems)
    {
        s->on_scene_unload(registry);
    }
}
/*
entity scene::add_entity(string_id name)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent);
    registry.emplace<model>(ent);
    registry.emplace<dirty_t>(ent);
    registry.emplace<enabled_t>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, entity parent)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, parent.get_handle());
    registry.emplace<model>(ent);
    registry.emplace<dirty_t>(ent);
    registry.emplace<enabled_t>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const vec2& position,
                         const vec2& scale, f32 rotation)
{
    auto ent = registry.create();
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, entt::null, entt::null);
    registry.emplace<model>(ent);
    registry.emplace<dirty_t>(ent);
    registry.emplace<enabled_t>(ent);
    return entity(&registry, ent);
}
entity scene::add_entity(string_id name, const vec2& position,
                         const vec2& scale, f32 rotation, entity parent)
{
    auto ent   = registry.create();
    auto order = parent.get_component<transform>().order + 1;
    registry.emplace<tag>(ent, name);
    registry.emplace<transform>(ent, position, scale, rotation, parent.get_handle(),
                                order);
    registry.emplace<model>(ent);
    registry.emplace<dirty_t>(ent);
    registry.emplace<enabled_t>(ent);
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
    registry.emplace_or_replace<dirty_t>(cloned_handle);
    return cloned;
}
void scene::remove_entity(entity& ent)
{
    registry.destroy(ent.get_handle());
}
*/
void scene::push2map(ptr<system> s, update_stage::frame_begin_t, size_type order)
{
    onFrameBeginMap[order] = s;
}
void scene::push2map(ptr<system> s, update_stage::update_t, size_type order)
{
    onUpdateMap[order] = s;
}
void scene::push2map(ptr<system> s, update_stage::physics_t, size_type order)
{
    onPhysicsMap[order] = s;
}
void scene::push2map(ptr<system> s, update_stage::frame_end_t, size_type order)
{
    onFrameEndMap[order] = s;
}
} // namespace lemon
