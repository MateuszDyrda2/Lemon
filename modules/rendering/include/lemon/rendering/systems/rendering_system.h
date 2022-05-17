#pragma once

#include "../renderer2d.h"

#include <lemon/core/math/vec2.h>
#include <lemon/core/string_id.h>
#include <lemon/events/event_bus.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC rendering_system : public system, listener
{
  public:
    rendering_system(service_registry& globalServices);
    ~rendering_system();
    void on_update(entity_registry& registry) override;
    void on_event(event* e) override;

  private:
    owned<renderer2d> spriteRenderer;
    ivec2 viewport;
    event_bus& ebus;
};
} // namespace lemon
