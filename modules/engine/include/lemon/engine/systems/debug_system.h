#pragma once

#include <lemon/scene/system.h>

#include <lemon/core/basic_types.h>
#include <lemon/events/event_bus.h>
#include <lemon/scene/scene.h>

namespace lemon {
class LEMON_PUBLIC debug_system : public system, listener
{
  public:
    debug_system(service_registry& globalRegistry);
    ~debug_system();
    void on_event(event* e) override;
    void on_update(entity_registry& registry) override;

  private:
    bool enabled;
    bool showColliders;
    bool showFPS;
    event_bus& ebus;
};
} // namespace lemon
