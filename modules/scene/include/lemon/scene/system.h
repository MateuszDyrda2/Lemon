#pragma once

#include <lemon/scene/entity.h>

#include <lemon/core/service_registry.h>

namespace lemon {
class system_registry;
/** Base system interface */
class LEMON_PUBLIC system
{
  public:
    system(service_registry& globalRegistry);
    virtual ~system() = default;

    virtual void on_scene_load(entity_registry& registry) { }

    virtual void on_update(entity_registry& registry) { }

    virtual void on_scene_unload(entity_registry& registry) { }
};
} // namespace lemon
