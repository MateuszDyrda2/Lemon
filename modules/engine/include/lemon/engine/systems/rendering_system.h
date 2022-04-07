#pragma once

#include <lemon/rendering/renderer2d.h>

#include <lemon/core/string_id.h>
#include <lemon/events/event_bus.h>
#include <lemon/scene/system.h>

namespace lemon {
class scene;
class LEMON_PUBLIC rendering_system : public system, listener
{
  public:
    rendering_system(ptr<scene> s, event_bus& ebus);
    ~rendering_system();
    void update(entity_registry& registry) override;
    void on_event(event* e) override;

  private:
    owned<renderer2d> spriteRenderer;
    entity mainCamera;
    event_bus& ebus;
};
} // namespace lemon
