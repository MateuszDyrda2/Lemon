#include "world2/world.h"

#include <algorithm>

namespace lemon {
using namespace std;
entityid world::create()
{
    return get_next();
}

void world::destroy(entityid entity)
{
    remove_entity(entity);
    for_each(
        pools.begin(), pools.end(),
        [entity](auto& p) {
            p.second.remove_entity(entity);
        });
}

void* world::add(entityid entity, componentdef def)
{
    if(auto iter = pools.find(def.id);
       iter != pools.end())
    {
        return iter->second.add_component(entity);
    }
    else
    {
        auto res = pools.emplace(
            def.id, component_pool(def));
        return res.first->second.add_component(entity);
    }
}

void* world::add(entityid entity, void* element, componentdef def)
{
    if(auto iter = pools.find(def.id);
       iter != pools.end())
    {
        return iter->second.cadd_component(entity, element);
    }
    else
    {
        auto res = pools.emplace(
            def.id, component_pool(def));
        return res.first->second.cadd_component(entity, element);
    }
}

bool world::all(entityid entity, std::initializer_list<componentid> components)
{
    return std::all_of(
        components.begin(), components.end(),
        [entity, this](auto component) {
            return pools.contains(component)
                   && pools.at(component).has_component(entity);
        });
}

bool world::any(entityid entity, std::initializer_list<componentid> components)
{
    return std::any_of(
        components.begin(), components.end(),
        [entity, this](auto component) {
            return pools.contains(component)
                   && pools.at(component).has_component(entity);
        });
}

void world::remove(entityid entity, componentid component)
{
    pools.at(component).remove_component(entity);
}

void* world::get(entityid entity, componentid component)
{
    return pools.at(component).get_component(entity);
}

entityid world::get_updated(entityid entity) const
{
    return entities[entity.id];
}

bool world::is_alive(entityid entity) const
{
    return entities[entity.id].id != entityid::null && entities[entity.id].alive;
}

bool world::is_enabled(entityid entity) const
{
    return entities[entity.id].id != entityid::null && entities[entity.id].enabled;
}

bool world::enable(entityid entity)
{
    auto prev                   = entities[entity.id].enabled;
    entities[entity.id].enabled = 1;
    return prev;
}

bool world::disable(entityid entity)
{
    auto prev                   = entities[entity.id].enabled;
    entities[entity.id].enabled = 0;
    return prev;
}

bool world::kill(entityid entity)
{
    auto prev                 = entities[entity.id].alive;
    entities[entity.id].alive = 0;
    return prev;
}

entityid world::get_next()
{
    if(next)
    {
        auto temp = next;
        next      = entities[next.id];
        ++temp.version;
        temp.enabled = 1;
        temp.alive   = 1;
        return temp;
    }
    const auto newEnt = entityid(entities.size());
    entities.emplace_back(newEnt);
    return newEnt;
}

void world::remove_entity(entityid entity)
{
    std::swap(entities[entity.id], next);
}

void* world::query(componentid component)
{
    if(auto iter = pools.find(component);
       iter != pools.end())
    {
        return iter->second.get_all();
    }
    return nullptr;
}
}