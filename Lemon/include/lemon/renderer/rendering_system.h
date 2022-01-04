#pragma once

#include "batch_renderer.h"

#include <lemon/core/string_id.h>
#include <lemon/event_system/event_handler.h>

#include <lemon/game/system.h>

namespace lemon {
class scene;
class rendering_system : public system
{
  public:
    rendering_system(ptr<scene> s);
    ~rendering_system();
    void update(entity_registry& registry) override;

  private:
    owned<batch_renderer> spriteRenderer;
    ptr<rendering_context> context;
    entity mainCamera;
    unsubscriber<int, int> vpResize;
};
} // namespace lemon
