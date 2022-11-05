/** @file scene.h
 * @brief Class representing a scene
 */
#pragma once

#include "core/hash_string.h"
#include "entity.h"
#include "entt/core/algorithm.hpp"
#include "entt/entity/fwd.hpp"
#include "entt/entity/registry.hpp"
#include "service_container.h"
#include "world/system.h"

#include <core/assert.h>
#include <core/defines.h>
#include <core/logger.h>
#include <core/utils.h>

#include <array>
#include <memory>
#include <string>
#include <vector>

#define ENT_NAME(_name) _name, _name##_hs

namespace lemon {
class LEMON_API scene
{
  public:
    scene(hash_str nameid, asset_storage& _assetStorage,
          scheduler& _scheduler, event_queue& _eventQueue,
          window& _window, input& _input,
          message_bus& _messageBus);
    ~scene();

    const auto& get_systems() const { return systems; }
    template<class S>
    scene& register_system();

    hash_str get_nameid() const { return nameid; }
    registry& get_registry() { return _registry; }
    const registry& get_registry() const { return _registry; }

    entity create_entity(const char* name, hash_str nameid);
    entity create_entity(const char* name, hash_str nameid, vec2 position);
    entity get_entity(entity_t handle);

    template<class Component, class... Other, class... Exclude>
    decltype(auto) view(entt::exclude_t<Exclude...> = {});

    template<class Component, class... Other, class... Exclude>
    decltype(auto) view(entt::exclude_t<Exclude...> = {}) const;

    template<class... Owned, class... Exclude>
    decltype(auto) group(entt::exclude_t<Exclude...> = {});

    template<class... Owned, class... Exclude>
    decltype(auto) group(entt::exclude_t<Exclude...> = {}) const;

    template<class... Component>
    decltype(auto) get(const entity_t e);

    template<class... Component>
    decltype(auto) get(const entity_t e) const;

    template<class To, class From>
    void sort();

    template<class Component, class Compare, class Sort = entt::std_sort, class... Args>
    void sort(Compare compare, Sort algo = Sort(), Args&&... args);

    template<class... Components>
    void clear();

    void destroy(const entity_t entity);
    void destroy_entity(const entity entity);
    template<class It>
    void destroy(It first, It second);

    service_container get_services()
    {
        return service_container{
            ._asset_storage = _assetStorage,
            ._scheduler     = _scheduler,
            ._event_queue   = _eventQueue,
            ._scene         = *this,
            ._window        = _window,
            ._input         = _input,
            ._message_bus   = _messageBus,
        };
    }

  private:
    hash_str nameid;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
    input& _input;
    message_bus& _messageBus;
    registry _registry;
    std::vector<std::unique_ptr<system>> systems;
};

template<class S>
scene& scene::register_system()
{
    systems.push_back(
        std::make_unique<S>(
            service_container{
                ._asset_storage = _assetStorage,
                ._scheduler     = _scheduler,
                ._event_queue   = _eventQueue,
                ._scene         = *this,
                ._window        = _window,
                ._input         = _input,
                ._message_bus   = _messageBus }));
    return *this;
}

template<class Component, class... Other, class... Exclude>
decltype(auto) scene::view(entt::exclude_t<Exclude...> e)
{
    return _registry.view<Component, Other...>(e);
}

template<class Component, class... Other, class... Exclude>
decltype(auto) scene::view(entt::exclude_t<Exclude...> e) const
{
    return _registry.view<Component, Other...>(e);
}

template<class... Owned, class... Exclude>
decltype(auto) scene::group(entt::exclude_t<Exclude...> e)
{
    return _registry.group<Owned...>(e);
}

template<class... Owned, class... Exclude>
decltype(auto) scene::group(entt::exclude_t<Exclude...> e) const
{
    return _registry.group<Owned...>(e);
}

template<class... Component>
decltype(auto) scene::get(const entity_t e)
{
    return _registry.get<Component...>(e);
}

template<class... Component>
decltype(auto) scene::get(const entity_t e) const
{
    return _registry.get<Component...>(e);
}

template<class To, class From>
void scene::sort()
{
    _registry.sort<To, From>();
}

template<class Component, class Compare, class Sort, class... Args>
void scene::sort(Compare compare, Sort algo, Args&&... args)
{
    _registry.sort<Component>(compare, algo, std::forward<Args>(args)...);
}

template<class... Components>
void scene::clear()
{
    _registry.clear<Components...>();
}

template<class It>
void scene::destroy(It first, It second)
{
    _registry.destroy(first, second);
}
}
