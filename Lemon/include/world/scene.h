/** @file scene.h
 * @brief Class representing a scene
 */
#pragma once

#include "core/hash_string.h"
#include "entity.h"
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
          window& _window, input& _input);
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
    service_container get_services()
    {
        return service_container{
            ._asset_storage = _assetStorage,
            ._scheduler     = _scheduler,
            ._event_queue   = _eventQueue,
            ._scene         = *this,
            ._window        = _window,
            ._input         = _input,
        };
    }

  private:
    hash_str nameid;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
    input& _input;
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
                ._input         = _input }));
    return *this;
}
}
