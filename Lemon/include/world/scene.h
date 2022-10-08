/** @file scene.h
 * @brief Class representing a scene
 */
#pragma once

#include "core/hash_string.h"
#include "entity.h"
#include "service_container.h"

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
/** @brief Execution stage of a system */
enum class execution_stage
{
    early_update, ///< 0
    update,       ///< 1
    late_update,  ///< 2
    render,       ///< 3
    none
};

class LEMON_API scene
{
  public:
    scene(hash_str nameid, asset_storage& _assetStorage,
          scheduler& _scheduler, event_queue& _eventQueue,
          window& _window);
    ~scene();

    void on_load();
    void update();
    void on_unload();
    const auto& get_systems() const { return systems; }
    template<class S>
    scene& register_system(execution_stage stage);

    hash_str get_nameid() const { return nameid; }
    registry& get_registry() { return _registry; }
    const registry& get_registry() const { return _registry; }

    entity create_entity(const char* name, hash_str nameid);
    entity create_entity(const char* name, hash_str nameid, vec2 position);
    service_container get_services()
    {
        return service_container{
            ._asset_storage = _assetStorage,
            ._scheduler     = _scheduler,
            ._event_queue   = _eventQueue,
            ._registry      = _registry,
            ._window        = _window,
        };
    }

  private:
    hash_str nameid;
    asset_storage& _assetStorage;
    scheduler& _scheduler;
    event_queue& _eventQueue;
    window& _window;
    registry _registry;
    std::array<std::vector<std::unique_ptr<system>>, 4> systems;
};

template<class S>
scene& scene::register_system(execution_stage stage)
{
    lemon_assert(enum_undelying(stage) < (int)systems.size());
    systems[enum_undelying(stage)].push_back(
        std::make_unique<S>(
            service_container{
                ._asset_storage = _assetStorage,
                ._scheduler     = _scheduler,
                ._event_queue   = _eventQueue,
                ._registry      = _registry,
                ._window        = _window }));
    return *this;
}
}
