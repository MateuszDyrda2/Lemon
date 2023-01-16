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
#include "world/entity_registry.h"
#include "world/system.h"

#include <core/assert.h>
#include <core/defines.h>
#include <core/logger.h>
#include <core/utils.h>

#include <array>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#define ENT_NAME(_name) _name, _name##_hs

namespace lemon {
/** A scene representation */
class LEMON_API scene
{
  public:
    /** Creates a new scene
     * @param nameid id of the scene
     * @param _assetStorage asset storage reference
     * @param _scheduler job scheduler reference
     * @param _eventQueue event queue reference
     * @param _window window reference
     * @param _input input manager reference
     * @param _messageBus message bus reference
     */
    scene(hashstr nameid, asset_storage& _assetStorage,
          scheduler& _scheduler, event_queue& _eventQueue,
          window& _window, input& _input,
          message_bus& _messageBus);
    ~scene();
    /** @return systems */
    const auto& get_systems() const { return systems; }
    /** @brief Register a system
     * @tparam S system type to register
     * @return this reference
     */
    template<class S>
    scene& register_system();
    /** @return nameid of the scene */
    hashstr get_nameid() const { return nameid; }
    /** @return scene registry */
    registry& get_registry() { return _registry; }
    /** @return scene registry */
    const registry& get_registry() const { return _registry; }
    /** @brief Creates an entity
     * @param name nameid of the newly created entity
     * @return new entity
     */
    entity create_entity(hashstr name);
    /** @brief Creates an entity
     * @param name nameid of the newly created entity
     * @param position entity position
     * @return new entity
     */
    entity create_entity(hashstr name, vec2 position);
    /** @brief Returns created entity
     * @param handle entity handle
     * @return entity
     */
    entity get_entity(entity_t handle);
    /** @brief Mounts the scene */
    void mount() noexcept;
    /** @brief Gets a view of type
     * @param Exclude list
     * @return view of types
     */
    template<class Component, class... Other, class... Exclude>
    decltype(auto) view(entt::exclude_t<Exclude...> = {});
    /** @brief Gets a view of type
     * @param Exclude list
     * @return view of types
     */
    template<class Component, class... Other, class... Exclude>
    decltype(auto) view(entt::exclude_t<Exclude...> = {}) const;
    /** @brief gets a group of type
     * @param exclude list
     * @return group of types
     */
    template<class... Owned, class... Exclude>
    decltype(auto) group(entt::exclude_t<Exclude...> = {});
    /** @brief gets a group of type
     * @param exclude list
     * @return group of types
     */
    template<class... Owned, class... Exclude>
    decltype(auto) group(entt::exclude_t<Exclude...> = {}) const;
    /** @brief Get component for entity when component size is == 0
     * @param e entity handle
     */
    template<class... Component>
    inline void get(const entity_t e)
        requires((sizeof...(Component) == 1) && (std::is_empty<Component...>::value))
    { }
    /** @brief Get components for entity
     * @param e entity handle
     * @return Component references
     */
    template<class... Component>
    decltype(auto) get(const entity_t e);
    /** @brief Get components for entity
     * @param e entity handle
     * @return Component references
     */
    template<class... Component>
    decltype(auto) get(const entity_t e) const;
    /** @brief Sort the registry */
    template<class To, class From>
    void sort();
    /** @brief Sort the components
     * @param compare compare function
     * @param algo sorting algorihtm
     * @param ...args argument to sort
     */
    template<class Component, class Compare, class Sort = entt::std_sort, class... Args>
    void sort(Compare compare, Sort algo = Sort(), Args&&... args);
    /** @brief Clears the component storages */
    template<class... Components>
    void clear();
    /** @brief Destroy entity
     * @param entity handle
     */
    void destroy(const entity_t entity);
    /** @brief Destroy entity
     * @param entity entity
     */
    void destroy_entity(const entity entity);
    /** @brief Destroy range
     * @param first start
     * @param second end
     */
    template<class It>
    void destroy(It first, It second);
    /** @return entity registry */
    entity_registry& get_entity_registry() { return _entityRegistry; }
    /** @return entity registry */
    const entity_registry& get_entity_registry() const { return _entityRegistry; }
    /** @return service registry*/
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
    hashstr nameid;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
    input& _input;
    message_bus& _messageBus;
    registry _registry;
    entity_registry _entityRegistry;
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
decltype(auto) scene::group(entt::get_t<Exclude...> e)
{
    return _registry.group<Owned...>(e);
}

template<class... Owned, class... Exclude>
decltype(auto) scene::group(entt::get_t<Exclude...> e) const
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
