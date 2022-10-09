#include <world/scene.h>

#include <world/components/entity_components.h>
#include <world/components/transform_components.h>

namespace lemon {
scene::scene(hash_str nameid,
             asset_storage& _assetStorage, scheduler& _scheduler,
             event_queue& _eventQueue, window& _window, input& _input):
    nameid(nameid),
    _assetStorage(_assetStorage),
    _scheduler(_scheduler),
    _eventQueue(_eventQueue),
    _window(_window),
    _input(_input)
{ }

scene::~scene()
{ }

entity scene::create_entity(const char* name, hash_str nameid)
{
    auto ent = _registry.create();
    _registry.emplace<transform>(ent);
    _registry.emplace<tag>(ent, name, nameid);
    _registry.emplace<dirty_t>(ent);
    _registry.emplace<model>(ent);
    return entity(&_registry, ent);
}

entity scene::create_entity(const char* name, hash_str nameid, vec2 position)
{
    auto ent = _registry.create();
    _registry.emplace<transform>(ent, position);
    _registry.emplace<tag>(ent, name, nameid);
    _registry.emplace<dirty_t>(ent);
    _registry.emplace<model>(ent);
    return entity(&_registry, ent);
}

entity scene::get_entity(entity_t handle)
{
    return entity(&_registry, handle);
}
void scene::destroy(const entity_t entity)
{
    _registry.destroy(entity);
}
void scene::destroy_entity(const entity entity)
{
    _registry.destroy(entity.get_handle());
}
}
