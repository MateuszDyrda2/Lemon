#pragma once

#include <lemon/core/system.h>
#include <lemon/scene/entity.h>

namespace lemon {
class system_registry;
/** Base system interface */
class LEMON_PUBLIC world_system : public system
{
  public:
    world_system()          = default;
    virtual ~world_system() = default;
    /** @brief Updates the components that belong to the system
     * @param registry entity registry
     */
    virtual void update(system_registry& sregistry, entity_registry& eregistry) { }
    virtual void on_scene_load() { }
    virtual void on_scene_unload() { }
};
} // namespace lemon
